#ifndef CRON_SCHEDULER_JOIN_THREADS_H
#define CRON_SCHEDULER_JOIN_THREADS_H

#include <vector>
#include<thread>

//make sure that all given threads are joined before object goes out of scope or is explicitly destroyed
class Join_threads {
    std::vector<std::thread> &threads;
public:
    explicit Join_threads(std::vector<std::thread> &threads_) :
            threads(threads_) {}

    ~Join_threads();
};


#endif //CRON_SCHEDULER_JOIN_THREADS_H
