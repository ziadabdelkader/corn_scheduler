#include "gtest/gtest.h"
#include "../src/job.h"
#include "../tasks.h"
#include "../src/thread_pool.h"
#include <fstream>

namespace scheduler_test {
    std::string read_file_content(const std::string &path) {
        std::ifstream file(path);
        std::string file_content((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
        return file_content;
    }

    TEST(scheduler, single_job) {
        // arrange
        const std::string output_file_name = "single_job_output.txt";
        const std::string message = "hello";
        Thread_pool scheduler{};
        std::vector<Job> jobs{
                Job{
                        1,
                        std::chrono::seconds(10),
                        std::chrono::seconds(2),
                        [&output_file_name, &message]() { write_to_file(output_file_name, message); }
                },

        };

        //act (add jobs to the scheduler and wait for job1 to run once)
        for (const auto &job: jobs)
            scheduler.submit(job);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        //assert
        EXPECT_TRUE(read_file_content(output_file_name) == message);

        //Tier down
        std::remove(output_file_name.c_str());
    }

    TEST(scheduler, single_job_twice) {
        // arrange
        const std::string output_file_name = "single_job_twice_output.txt";
        const std::string message = "hello";
        Thread_pool scheduler{};
        std::vector<Job> jobs{
                Job{
                        1,
                        std::chrono::seconds(2),
                        std::chrono::seconds(2),
                        [&output_file_name, &message]() { write_to_file(output_file_name, message); }
                },

        };

        //act (add jobs to the scheduler and wait for job1 to run twice)
        for (const auto &job: jobs)
            scheduler.submit(job);
        std::this_thread::sleep_for(std::chrono::seconds(3));

        //assert
        EXPECT_TRUE(read_file_content(output_file_name) == (message + message));

        //Tier down
        std::remove(output_file_name.c_str());
    }

    TEST(scheduler, two_jobs) {
        // arrange
        const std::string job1_output_file = "job1.txt";
        const std::string job2_output_file = "job2.txt";
        const std::string message = "hello";
        Thread_pool scheduler{};
        std::vector<Job> jobs{
                Job{
                        1,
                        std::chrono::seconds(4),
                        std::chrono::seconds(2),
                        [&job1_output_file, &message]() { write_to_file(job1_output_file, message); }
                },
                Job{
                        2,
                        std::chrono::seconds(2),
                        std::chrono::seconds(2),
                        [&job2_output_file, &message]() { write_to_file(job2_output_file, message); }
                },
        };

        //act
        for (const auto &job: jobs)
            scheduler.submit(job);
        std::this_thread::sleep_for(std::chrono::seconds(3));


        //assert (job1 should run once while job2 should run twice)
        EXPECT_TRUE(read_file_content(job1_output_file) == message);
        EXPECT_TRUE(read_file_content(job2_output_file) == (message + message));

        //Tier down
        std::remove(job1_output_file.c_str());
        std::remove(job2_output_file.c_str());
    }
}
