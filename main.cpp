#include <vector>
#include "tasks.h"
#include "src/job.h"
#include "src/thread_pool.h"
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

int main() {
    plog::init(plog::info, "Logfile.txt");
    Thread_pool scheduler{};

    std::vector<Job> jobs{
            Job(
                    1,
                    std::chrono::seconds (10),
                    std::chrono::seconds (10),
                    []() { write_many("job1.txt", 1e1); }),
            Job(
                    2,
                    std::chrono::seconds (20),
                    std::chrono::seconds (20),
                    []() { mod_many("job2.txt", 1e1); }),
            Job(
                    3,
                    std::chrono::seconds (5),
                    std::chrono::seconds (5),
                    []() { write_to_file("job3.txt", "hello world\n"); }),
    };

    for(const auto& job: jobs){
        scheduler.submit(job);
    }
    return 0;
}