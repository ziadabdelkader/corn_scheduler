#ifndef CRON_SCHEDULER_THREAD_SAFE_HEAP_H
#define CRON_SCHEDULER_THREAD_SAFE_HEAP_H

#include <mutex>
#include <queue>
#include "job.h"

struct Event {
    std::chrono::time_point<std::chrono::system_clock> time;
    Job job;

    //gives higher priority to sooner events
    bool operator<(const Event &rhs) const {
        return time > rhs.time;
    }
};

// maintains the next time to execute each job
class Thread_safe_heap {
private:
    mutable std::mutex mut;
    std::priority_queue<Event> event_queue;
public:
    Thread_safe_heap() = default;

    void push(Job new_job);

    //tries to pop a job from the queue if it is time to execute
    // and push back the next scheduled time atomically
    bool try_pop_and_push(Job &job);

    bool empty() const;
};

#endif //CRON_SCHEDULER_THREAD_SAFE_HEAP_H
