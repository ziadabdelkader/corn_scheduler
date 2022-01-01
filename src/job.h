#ifndef CRON_SCHEDULER_JOB_H
#define CRON_SCHEDULER_JOB_H

#include <cstdint>
#include <chrono>
#include <functional>
#include <utility>

typedef std::function<void()> Task;

class Job {
public:
    int32_t id{};
    std::chrono::milliseconds frequency{};
    std::chrono::milliseconds expected_interval{};

    Job() = default;

    template<typename Rep1, typename Period1, typename Rep2, typename Period2>
    Job(int32_t id,
        std::chrono::duration<Rep1, Period1> frequency,
        std::chrono::duration<Rep2, Period2> expected_interval,
        Task fun): id(id),
                   frequency(frequency),
                   expected_interval(expected_interval),
                   fun(std::move(fun)) {};


    void execute();

private:
    Task fun;

};


#endif //CRON_SCHEDULER_JOB_H
