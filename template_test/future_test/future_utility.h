#pragma once

#include <atomic>
#include <vector>
#include "future.h"

namespace internal {
template <typename T>
class DoWithKeeper {
 public:
  explicit DoWithKeeper(T &&value) noexcept { value_ = std::move(value); }
  T &GetValue() noexcept { return value_; }

 private:
  T value_;
};
}  // namespace internal

/**
 * @brief Move a object to heap memory, and hold it until
 * func's future was resolved.
 * The object will be passed into func as lvalue reference.
 *
 * @tparam T
 * @tparam Func
 * @tparam std::result_of<Func(T &)>::type
 * @param value
 * @param func
 * @return R
 */
template <typename T, typename Func, typename R = typename std::result_of<Func(T &)>::type>
R DoWith(T &&value, Func &&func) {
  auto keeper = std::make_unique<internal::DoWithKeeper<T>>(std::move(value));
  return func(keeper->GetValue()).Then([kp = std::move(keeper)](R &&future) { 
	  return std::move(future); 
  });
}

/**
 * @brief Execute func repeatedly until stop function return true.
 * If the func return an exception future, the loop will terminate
 * immediately and return a same exception.
 *
 * @tparam Func
 * @tparam T
 * @tparam std::result_of<Func(T &&...)>::type
 * @param stop
 * @param func
 * @return Future<>
 */
template <typename Func, typename... T, typename R = typename std::result_of<Func(T &&...)>::type>
Future<> DoUntil(std::function<bool(void)> stop, Func &&func) {
  if (stop()) {
    return MakeReadyFuture<>();
  }

  return func().Then([stop = std::move(stop), func = std::move(func)](R &&fut) mutable {
    if (fut.is_failed()) {
      return MakeExceptionFuture<>(fut.GetException());
    }
    return DoUntil(stop, std::forward<Func>(func));
  });
}

namespace internal {

/*
  Reference: https://codereview.stackexchange.com/questions/51407/stdtuple-foreach-implementation
*/
template <typename Tuple, typename F, std::size_t... Indices>
constexpr void for_each_impl(Tuple &&tuple, F &&f, std::index_sequence<Indices...>) {
  //原来这里是在遍历，是啥语法?
  int ignore[] = {1, (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...};
  (void)ignore;
}

template <typename Tuple, typename F>
constexpr void for_each(Tuple &&tuple, F &&f) {
  constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
  for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<N>{});
}

template <typename Tuple>
class WhenAllFunction {
 public:
  WhenAllFunction(Promise<Tuple> pr, Tuple &futs, const std::shared_ptr<std::atomic_uint32_t> &counter)
      : pr_(pr), futs_(futs), counter_(counter) {}

  template <typename Fut>
  void operator()(Fut &fut) {
    auto &futs = futs_;
    auto &input_fut = fut;
    DoWith(std::move(fut), [&futs, &input_fut, counter = counter_, pr = pr_](Fut &fut) mutable {
      return fut.Then([&futs, &input_fut, counter, pr](std::remove_reference_t<Fut> &&result) mutable {
		//n个参数设置n次回调
        input_fut = std::move(result);
        auto current = (*counter).fetch_add(1);
        if (current + 1 == std::tuple_size<Tuple>::value) {
		  //当n次回调都满了的时候，即设置好了值，然后就会调用最初的回调函数
          pr.SetValue(std::move(futs));
        }
        return MakeReadyFuture<>();
      });
    });
  }

 private:
  Promise<Tuple> pr_;
  Tuple &futs_;
  std::shared_ptr<std::atomic_uint32_t> counter_;
};

}  // namespace internal

/**
 * @brief Waiting for mutiple futures at same time.
 * When all futures were resolved(whatere ready or failed), this function's future
 * will become ready with a tuple of all futures.
 *
 * @tparam FutsType
 * @param futs
 * @return Future<std::tuple<FutsType...>>
 */
template <typename... FutsType>
Future<std::tuple<FutsType...>> WhenAll(FutsType &&... futs) {
  if constexpr (std::tuple_size<std::tuple<FutsType...>>::value == 0) {
    return MakeReadyFuture<std::tuple<>>(std::tuple<>());
  } else {
    auto input = std::make_tuple(std::move(futs)...);

    return DoWith(std::move(input), [](std::tuple<FutsType...> &futs) {
      Promise<std::tuple<FutsType...>> pr;
      auto counter = std::make_shared<std::atomic_uint32_t>(0);
      internal::for_each(futs, internal::WhenAllFunction<std::tuple<FutsType...>>(pr, futs, counter));
      return pr.get_future();
    });
  }
}

/**
 * @brief This function is as same as @ref WhenAll(FutsType &&... futs) , but accept
 * two interators to set input futures.
 *
 *
 * @tparam InputIterator
 * @tparam std::iterator_traits<InputIterator>::value_type
 * @param first
 * @param last
 * @return Future<std::vector<FutureType>>
 */
template <typename InputIterator, typename FutureType = typename std::iterator_traits<InputIterator>::value_type>
Future<std::vector<FutureType>> WhenAll(InputIterator first, InputIterator last) {
  std::vector<FutureType> result;
  if (last - first == 0) {
    return MakeReadyFuture<std::vector<FutureType>>(std::move(result));
  } else {
    return DoWith(std::move(result), [last, first](std::vector<FutureType> &result) mutable {
      result.resize(last - first);
      Promise<std::vector<FutureType>> pr;
      auto counter = std::make_shared<std::atomic_uint32_t>(0);
      uint32_t size = last - first;
      for (auto it = first; it != last; ++it) {
        uint32_t idx = it - first;
        it->Then([counter, idx, &result, size, pr](FutureType &&fut) mutable {
          auto current = (*counter).fetch_add(1);
          result[idx] = std::move(fut);
          if (current + 1 == size) {
            pr.SetValue(std::move(result));
          }
          return MakeReadyFuture<>();
        });
      }
      return pr.get_future();
    });
  }
}
