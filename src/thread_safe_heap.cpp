#include <iostream>
#include "thread_safe_heap.h"

void Thread_safe_heap::push(Event new_event) {
    std::lock_guard<std::mutex> lk(mut);
    std::cout << "pushing to the queue" << std::endl;
    event_queue.push(std::move(new_event));
}

bool Thread_safe_heap::try_pop_and_push(Event &event) {
    std::lock_guard<std::mutex> lk(mut);

    //check if it's time to perform next task
    if (event_queue.empty() || event_queue.top().time > std::chrono::system_clock::now())
        return false;

    std::cout << "removing from the queue" << std::endl;
    //remove the task from the queue
    event = event_queue.top();
    event_queue.pop();

    //add the task back to the queue with the next scheduled time
    Event new_event = event;
    new_event.time += new_event.job.frequency;
    event_queue.push(std::move(new_event));

    return true;
}

bool Thread_safe_heap::empty() const {
    std::lock_guard<std::mutex> lk(mut);
    return event_queue.empty();
}
