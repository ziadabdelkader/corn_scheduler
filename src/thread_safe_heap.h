#ifndef CRON_SCHEDULER_THREAD_SAFE_HEAP_H
#define CRON_SCHEDULER_THREAD_SAFE_HEAP_H

#include <mutex>
#include <queue>
#include "job.h"

struct Event {
    std::chrono::time_point<std::chrono::system_clock> time;
    Job job;

    bool operator<(const Event &rhs) const {
        return time > rhs.time;
    }
};

class Thread_safe_heap {
private:
    mutable std::mutex mut;
    std::priority_queue<Event> event_queue;
public:
    Thread_safe_heap() = default;

    void push(Event new_event);

    bool try_pop_and_push(Event &event);

    bool empty() const;
};

#endif //CRON_SCHEDULER_THREAD_SAFE_HEAP_H
