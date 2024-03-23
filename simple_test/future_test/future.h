#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>
#include <condition_variable>
#include <mutex>
#include "function_traits.h"
#include "continuation.h"
#include "exception.h"

template <typename... T>
class Promise;
template <typename... T>
class Future;
struct MakeReadyFutureHelper {};
struct MakeExceptionFutureHelper {};

template <typename... T>
Future<T...> MakeReadyFuture(T &&... v) {
  return Future<T...>(MakeReadyFutureHelper(), std::move(v)...);
}

template <typename... T>
Future<T...> MakeExceptionFuture(const Exception &e) {
  return Future<T...>(e);
}

template <typename... T>
class Continuation : public ContinuationBase {
 public:
  virtual ~Continuation() noexcept {}
  virtual void SetValue(std::tuple<T...> &&value) noexcept {      // when superior future is ready, set value that callback will reference
    ready_ = true;
    value_ = std::move(value);
  }

  virtual void SetException(Exception &&e) {                      // when superior future is failed, set exception that callback will reference
    failed_ = true;
    e_ = e;
  }

  virtual void Schedule(std::unique_ptr<Continuation<T...>> &&task) {
    const auto &scheduler = get_thread_local_scheduler();
    if (scheduler != nullptr) {                           // if have a scheduler, callback should be invoked by scheduler
      scheduler(std::move(task));
    } else {
      task->Run();
    }
  }

 protected:
  bool ready_ = false;
  bool failed_ = false;
  std::tuple<T...> value_;
  Exception e_;
};


template <typename PromiseType, typename FutureType>
void ChainFuture(FutureType &&fut, PromiseType &&promise) {
  // combine superior promise with value or exception, make state clearly
  if (fut.is_ready()) {
    promise.SetValue(fut.GetValue());             
  } else if (fut.is_failed()) {
    promise.SetException(fut.GetException());
  } else {                                            // when async, superior promise's state may not clearly by now 
    using ResultFutureType = typename PromiseType::FutureType;
    fut.Then([pr = std::move(promise)](ResultFutureType &&result) mutable {     // use superior callback's return to registry a new callback for 
                                                                                // setting state of superior then's promise
      if (result.is_ready()) {
        pr.SetValue(result.GetValue());
      } else {
        pr.SetException(result.GetException());
      }
      return MakeReadyFuture<>();                           // help to deduce then's return type
    });
  }
}

template <typename Func, typename PromiseType, typename... T>
class ContinuationWithValue : public Continuation<T...> {
  using BaseClass = Continuation<T...>;

 public:
  ContinuationWithValue(Func &&func, PromiseType &&promise)
      : func_(std::forward<Func>(func)), promise_(std::forward<PromiseType>(promise)) {}

  virtual ~ContinuationWithValue() = default;

  void Run() {
    if (BaseClass::failed_) {
      promise_.SetException(BaseClass::e_);       // combine exception with promise, and triger callback
      return;                                     // when callback param-type is value, subsequent callback will be ignored
    }
    auto r = std::apply<Func, std::tuple<T...>>(std::move(func_), std::move(BaseClass::value_));    // invoke callback use values as parameter
    ChainFuture(std::move(r), std::move(promise_));    // use pre callback's return future and current promise as param
  }

 private:
  Func func_;
  PromiseType promise_;
};

template <typename Func, typename PromiseType, typename... T>
class ContinuationWithFuture : public Continuation<T...> {
  using BaseClass = Continuation<T...>;

 public:
  ContinuationWithFuture(Func &&func, PromiseType &&promise)
      : func_(std::forward<Func>(func)), promise_(std::forward<PromiseType>(promise)) {}

  virtual ~ContinuationWithFuture() = default;

  void Run() {
    Promise<T...> promise;            // construct a temp peomise
  
    if (BaseClass::failed_) {         
      promise.SetException(BaseClass::e_);
    } else {
      promise.SetValue(std::move(BaseClass::value_));
    }
    auto r = func_(std::move(promise.get_future()));
    ChainFuture(std::move(r), std::move(promise_));         
  }

 private:
  Func func_;
  PromiseType promise_;
};

template <typename... T>
class FutureImpl : public std::enable_shared_from_this<FutureImpl<T...>> {
  friend class Promise<T...>;
  friend class Future<T...>;

 public:
  FutureImpl() = default;
  ~FutureImpl() noexcept = default;

  bool is_ready() const noexcept { return state_->ready; }

  bool is_failed() const noexcept { return state_->failed; }

  bool Wait(const uint32_t timeout) noexcept {
    std::unique_lock<std::mutex> lock(state_->mutex_);
    return state_->cond_promise_ready_.wait_for(lock, std::chrono::milliseconds(timeout), [&] { return state_->ready == true || state_->failed == true; });
  }

  std::tuple<T...> &&GetValue() noexcept {
    state_->ready = false;
    return std::move(state_->value);
  }

  Exception GetException() noexcept {
    state_->failed = false;
    return state_->exception;
  }

  template <typename FutureType,
            typename PromiseType = typename FutureType::PromiseType,
            typename Func = std::function<FutureType(T &&...)>>
  void SetCallback(PromiseType promise, Func &&func) {
    state_->callback =
        std::make_unique<ContinuationWithValue<Func, PromiseType, T...>>(std::forward<Func>(func), std::move(promise));
    std::atomic_thread_fence(std::memory_order_release);
    state_->has_callback = true;
    if (is_ready() || is_failed()) {
      TrySchedule();
    }
  }

  template <typename FutureType,
            typename PromiseType = typename FutureType::PromiseType,
            typename Func = std::function<FutureType(Future<T...> &&)>>
  void SetCallbackWrapped(PromiseType promise, Func &&func) {
    state_->callback =
        std::make_unique<ContinuationWithFuture<Func, PromiseType, T...>>(std::forward<Func>(func), std::move(promise));
    std::atomic_thread_fence(std::memory_order_release);
    state_->has_callback = true;
    if (is_ready() || is_failed()) {
      TrySchedule();
    }
  }

  void SetValue(std::tuple<T...> &&value) {
    state_->value = std::move(value);
    std::atomic_thread_fence(std::memory_order_release);
    std::lock_guard<std::mutex> lock(state_->mutex_);
    state_->ready = true;
    state_->cond_promise_ready_.notify_all();
    TrySchedule();
  }

  void SetException(const Exception &e) {
    state_->exception = e;
    std::atomic_thread_fence(std::memory_order_release);
    std::lock_guard<std::mutex> lock(state_->mutex_);
    state_->failed = true;
    state_->cond_promise_ready_.notify_all();
    TrySchedule();
  }

 private:
 /** When promise get avalue or exception, try to schedule all the callback one by one.
  * And this step only care about the direct callback, inderect ones used same mechanism.
 */
  
  void TrySchedule() {
    if (state_->has_callback) {
      if (is_ready()) {
        if (state_->schedule_flag.test_and_set() == false) {
          std::atomic_thread_fence(std::memory_order_acquire);
          state_->callback->SetValue(GetValue());                  // pass value to the callback
          state_->callback->Schedule(std::move(state_->callback)); // do not invoke callback immediately, use schdule to determine
        }
      } else if (is_failed()) {
        if (state_->schedule_flag.test_and_set() == false) {
          std::atomic_thread_fence(std::memory_order_acquire);
          state_->callback->SetException(GetException());
          state_->callback->Schedule(std::move(state_->callback));
        }
      }
    }
  }

  struct State {
    bool ready = false;
    bool failed = false;
    bool has_callback = false;
    std::tuple<T...> value;
    Exception exception;
    std::unique_ptr<Continuation<T...>> callback;
    std::atomic_flag schedule_flag = ATOMIC_FLAG_INIT;
    std::condition_variable cond_promise_ready_;
    std::mutex mutex_;
  };

  /* Decrease times of memory allocation. */
  State state_data_;
  State *state_ = &state_data_;
  /* It's for holding memory of future state after move state. */
  std::shared_ptr<FutureImpl<T...>> state_holder_ = nullptr;
};
template <typename T>
struct is_future {
  static const bool value = false;
};
template <typename... T>
struct is_future<Future<T...>> {
  static const bool value = true;
};
template <typename... T>
struct is_future<Future<T...> &&> {
  static const bool value = true;
};

template <typename... T>
class Future {
 public:
  using ValueType = std::tuple<T...>;
  using Type = Future<T...>;
  using PromiseType = Promise<T...>;

  Future() = default;

  explicit Future(std::shared_ptr<FutureImpl<T...>> future) noexcept : future_(future) {}

  explicit Future(MakeReadyFutureHelper no_sense, T &&... v) {
    future_ = std::make_shared<FutureImpl<T...>>();
    std::tuple<T...> tuple = std::make_tuple<T...>(std::move(v)...);
    future_->SetValue(std::move(tuple));
  }

  explicit Future(const Exception e) {
    future_ = std::make_shared<FutureImpl<T...>>();
    future_->SetException(e);
  }

  Future(Future &&o) noexcept {
    future_ = o.future_;
    o.future_ = nullptr;
  }

  Future &operator=(Future &&o) noexcept {
    future_ = o.future_;
    o.future_ = nullptr;
    return *this;
  }

  bool is_ready() const noexcept { return future_->is_ready(); }

  bool is_failed() const noexcept { return future_->is_failed(); }
  /**
   * @brief Add a callback to future.
   *
   * If the callback only has one parameter and its type is @ref Future<T...>,
   * when the future becomes ready or failed, the callback will be called and
   * the future will be passed as pararemter to callback.
   *
   * If the callback has more than one parameters or its parameter type is not
   * future<T...>, when this future is ready, this callback will be called with
   * future's value as parameters. And if this future is failed, this callback
   * will be ignored.
   *
   * @tparam Func
   * @param func
   */
  template <typename Func>
  auto Then(Func &&func) {
    using func_type = std::decay_t<Func>;
    using tuple_type = typename function_traits<func_type>::tuple_type;
    if constexpr (function_traits<Func>::arity == 1) {
      using first_arg_type = typename std::tuple_element<0, tuple_type>::type;
      if constexpr (is_future<std::decay_t<first_arg_type>>::value) {
        return InnerThenWrapped(std::forward<Func>(func));
      } else {
        return InnerThen(std::forward<Func>(func));
      }
    } else {
      return InnerThen(std::forward<Func>(func));
    }
  }

  /**
   * @brief Wait result the future to return
   *
   * @param milliseconds that to be waited
   *
   * @return false when timeout, true when result ready or exception occured
   */
  bool Wait(const uint32_t timeout = UINT32_MAX) noexcept { return future_->Wait(timeout); }

  /**
   * @brief Get value from a ready future, after call this function,
   * this future is not ready anymore.
   *
   * @return std::tuple<T...>
   */
  std::tuple<T...> GetValue() noexcept { return std::move(future_->GetValue()); }

  /**
   * @brief Get exception from a failed future, after call this function,
   * this future is not failed anymore.
   *
   * @return Exception
   */
  Exception GetException() noexcept { return future_->GetException(); }

 private:
  /**
   * @brief Add a callback to future.
   * When this future is ready, this callback will run with future value as
   * parameters. If this future is failed, this callback will be ignored.
   *
   * @tparam Func
   * @tparam std::result_of<Func(T &&...)>::type
   * @param func
   * @return R
   */
  template <typename Func, typename R = typename std::result_of<Func(T &&...)>::type>
  R InnerThen(Func &&func) {
    typename R::PromiseType promise;                                // transport a empty promise to callback                                    
    future_->template SetCallback<R>(promise, std::forward<Func>(func));        
    return promise.get_future();
  }

  /**
   * @brief Add a callback to future.
   * When this future is ready or failed, this callback will run with this
   * future as parameter. Callback should call @ref is_failed to check is ready
   * or failed first.
   * @tparam Func
   * @tparam std::result_of<Func(Future<T...> &&)>::type
   * @param func
   * @return R
   */
  template <typename Func, typename R = typename std::result_of<Func(Future<T...> &&)>::type>
  R InnerThenWrapped(Func &&func) {
    typename R::PromiseType promise;
    future_->template SetCallbackWrapped<R>(promise, std::forward<Func>(func));
    return promise.get_future();
  }

  std::shared_ptr<FutureImpl<T...>> future_ = nullptr;
};

template <typename... T>
class Promise {
 public:
  using FutureType = Future<T...>;
  Promise() { future_ = std::make_shared<FutureImpl<T...>>(); }

  void SetValue(T &&... value) {
    std::tuple<T...> tuple = std::make_tuple<T...>(std::move(value)...);
    future_->SetValue(std::move(tuple));
  }

  void SetValue(std::tuple<T...> &&value) { future_->SetValue(std::move(value)); }

  void SetException(const Exception &e) { future_->SetException(e); }

  bool is_ready() { return future_->is_ready(); }

  bool is_failed() { return future_->is_failed(); }

  Future<T...> get_future() noexcept { return Future<T...>(future_); }

 private:
  std::shared_ptr<FutureImpl<T...>> future_ = nullptr;
  friend class Future<T...>;
};

