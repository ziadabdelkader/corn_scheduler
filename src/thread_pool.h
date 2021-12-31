#ifndef CRON_SCHEDULER_THREAD_POOL_H
#define CRON_SCHEDULER_THREAD_POOL_H

#include <vector>
#include <thread>
#include <iostream>
#include "thread_safe_heap.h"
#include "join_threads.h"

class Thread_pool {
public:
    Thread_pool();

    void submit(Job job);

private:
    Thread_safe_heap event_queue;
    std::vector<std::thread> threads;
    Join_threads joiner;

    void worker_thread();

};


#endif //CRON_SCHEDULER_THREAD_POOL_H
