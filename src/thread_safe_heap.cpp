#include "thread_safe_heap.h"

void Thread_safe_heap::push(Job new_job) {
    std::lock_guard<std::mutex> lk(mut);
    event_queue.push(Event{std::chrono::system_clock::now() ,std::move(new_job)});
}

bool Thread_safe_heap::try_pop_and_push(Job &job) {
    std::lock_guard<std::mutex> lk(mut);

    //check if it's time to perform next task
    if (event_queue.empty() || event_queue.top().time > std::chrono::system_clock::now())
        return false;

    //remove the task from the queue
    Event event = event_queue.top();
    job = event.job;
    event_queue.pop();

    //add the task back to the queue with the next scheduled time
    event.time += event.job.frequency;
    event_queue.push(std::move(event));

    return true;
}

bool Thread_safe_heap::empty() const {
    std::lock_guard<std::mutex> lk(mut);
    return event_queue.empty();
}
