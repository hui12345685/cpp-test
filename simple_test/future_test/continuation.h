#pragma once

#include <functional>
#include <memory>
#include <tuple>

class ContinuationBase {
 public:
  virtual ~ContinuationBase() noexcept {}
  virtual void Run() = 0;
};

using Scheduler = std::function<void(std::unique_ptr<ContinuationBase> &&)>;

const Scheduler &get_thread_local_scheduler();
void set_thread_local_scheduler(Scheduler &&scheduler);

