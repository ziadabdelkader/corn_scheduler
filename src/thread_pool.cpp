#include "thread_pool.h"

void Thread_pool::worker_thread() {
    while (!done) {
        Job job;
        if (event_queue.try_pop_and_push(job)) {
            job.execute();
        } else {
            std::this_thread::yield();
        }
    }
}

Thread_pool::Thread_pool() :
        done(false), joiner(threads) {
    unsigned const thread_count = std::thread::hardware_concurrency();
    try {
        for (unsigned i = 0; i < thread_count; ++i) {
            threads.emplace_back(&Thread_pool::worker_thread, this);
        }
    }
    catch (...) {
        done=true;
        throw;
    }
}

void Thread_pool::submit(Job job) {
    event_queue.push(std::move(job));
}

Thread_pool::~Thread_pool() {
    done = true;
}
