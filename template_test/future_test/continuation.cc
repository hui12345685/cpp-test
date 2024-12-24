#include "continuation.h"

thread_local Scheduler kThreadLocalScheduler = nullptr;
const Scheduler &get_thread_local_scheduler() { return kThreadLocalScheduler; }
void set_thread_local_scheduler(Scheduler &&scheduler) { kThreadLocalScheduler = std::move(scheduler); }