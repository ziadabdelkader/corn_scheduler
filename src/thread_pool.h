#ifndef CRON_SCHEDULER_THREAD_POOL_H
#define CRON_SCHEDULER_THREAD_POOL_H

#include <vector>
#include <thread>
#include <atomic>
#include "thread_safe_heap.h"
#include "join_threads.h"

class Thread_pool {
public:
    Thread_pool();

    void submit(Job job);

    ~Thread_pool();

private:
    std::atomic_bool done;
    Thread_safe_heap event_queue;
    std::vector<std::thread> threads;
    Join_threads joiner; //join threads on destructor

    void worker_thread();

};


#endif //CRON_SCHEDULER_THREAD_POOL_H
