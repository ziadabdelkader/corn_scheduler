#include "thread_pool.h"

void Thread_pool::worker_thread() {
    while (true) {
        Event event;
        if (event_queue.try_pop_and_push(event)) {
            std::cout << "running task\n";
            event.job.execute();
            std::cout << "task done\n";
        } else {
            std::this_thread::yield();
        }
    }
}

Thread_pool::Thread_pool() :
        joiner(threads) {
    unsigned const thread_count = std::thread::hardware_concurrency();
    std::cout << thread_count << std::endl;
    try {
        for (unsigned i = 0; i < thread_count; ++i) {
            threads.emplace_back(&Thread_pool::worker_thread, this);
        }
    }
    catch (...) {
        throw;
    }
}

void Thread_pool::submit(Job job) {
    std::cout << "submitting event\n";
    event_queue.push(Event{
            std::chrono::system_clock::now(),
            std::move(job)
    });
}
