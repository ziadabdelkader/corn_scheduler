#include <thread>
#include <vector>
#include "gtest/gtest.h"
#include "../src/thread_safe_heap.h"

namespace thread_safe_heap_test {

    TEST(thread_safe_heap, single_thread_one_push) {

        //arrange
        Job job{
                1,
                std::chrono::seconds(1),
                std::chrono::seconds(1),
                []() {}
        };
        Thread_safe_heap heap{};

        //act
        heap.push(job);

        //assert
        Job job_returned;
        EXPECT_TRUE(heap.try_pop_and_push(job_returned));
        EXPECT_TRUE(job.id == job_returned.id);
        EXPECT_FALSE(heap.try_pop_and_push(job_returned));
    }

    TEST(thread_safe_heap, single_thread_one_push_freq0) {

        //arrange
        Job job{
                1,
                std::chrono::seconds(0),
                std::chrono::seconds(1),
                []() {}
        };
        Thread_safe_heap heap{};

        //act
        heap.push(job);

        //assert
        Job job_returned;
        EXPECT_TRUE(heap.try_pop_and_push(job_returned));
        EXPECT_TRUE(job.id == job_returned.id);
        EXPECT_TRUE(heap.try_pop_and_push(job_returned));
    }

    TEST(thread_safe_heap, multi_thread_push) {
        const int32_t threads_count = 100;
        const int32_t jobs_per_thread = 100;

        //arrange
        Thread_safe_heap heap{};
        std::vector<std::thread> threads(threads_count);

        //act
        for (int32_t i = 0; i < threads_count; i++) {
            threads[i] = std::thread([&heap, i]() {
                for (int32_t j = 0; j < jobs_per_thread; j++) {
                    heap.push(Job{
                            i * jobs_per_thread + j, //generates unique id for each job
                            std::chrono::hours(1),
                            std::chrono::hours(1),
                            []() {}
                    });
                }
            });
        }

        for (int32_t i = 0; i < threads_count; i++) {
            if (threads[i].joinable())
                threads[i].join();
        }

        //assert
        std::vector<bool> present_ids(threads_count * jobs_per_thread, false);
        Job job;
        for (int32_t i = 0; i < threads_count * jobs_per_thread; i++) {
            EXPECT_TRUE(heap.try_pop_and_push(job));
            present_ids[job.id] = true;
        }
        EXPECT_FALSE(heap.try_pop_and_push(job));
        for (bool is_present: present_ids) {
            EXPECT_TRUE(is_present);
        }
    }

    TEST(thread_safe_heap, multi_thread_pop) {
        const int32_t threads_count = 100;
        const int32_t jobs_count = 10000;

        //arrange
        Thread_safe_heap heap{};
        std::vector<std::thread> threads(threads_count);
        for (int32_t i = 0; i < jobs_count; i++) {
            heap.push(Job{
                    i,
                    std::chrono::hours(1),
                    std::chrono::hours(1),
                    []() {}
            });
        }

        //act
        std::vector<bool> present_ids(jobs_count, false);
        for (int32_t i = 0; i < threads_count; i++) {
            threads[i] = std::thread([&heap, &present_ids]() {
                Job job;
                while (heap.try_pop_and_push(job)) {
                    present_ids[job.id] = true; //this will be thread safe as job.id is unique
                }
            });
        }

        for (int32_t i = 0; i < threads_count; i++) {
            if (threads[i].joinable())
                threads[i].join();
        }

        //assert
        for (bool is_present: present_ids) {
            EXPECT_TRUE(is_present);
        }
    }

    TEST(thread_safe_heap, multi_thread_push_and_pop) {
        const int32_t threads_count = 100;
        const int32_t jobs_per_thread = 100;

        //arrange
        Thread_safe_heap heap{};
        std::vector<std::thread> threads(threads_count);
        std::vector<bool> present_ids(threads_count * jobs_per_thread, false);

        //act and assert
        for (int32_t i = 0; i < threads_count; i++) {
            threads[i] = std::thread([&heap, &present_ids, i]() {
                for (int32_t j = 0; j < jobs_per_thread; j++) {
                    heap.push(Job{
                            i * jobs_per_thread + j, //generates unique id for each job
                            std::chrono::hours(1),
                            std::chrono::hours(1),
                            []() {}
                    });
                }

                for (int32_t j = 0; j < jobs_per_thread; j++) {
                    Job job;
                    EXPECT_TRUE(heap.try_pop_and_push(job));
                    present_ids[job.id] = true; //this will be thread safe as job.id is unique
                }

            });
        }

        for (int32_t i = 0; i < threads_count; i++) {
            if (threads[i].joinable())
                threads[i].join();
        }

        //assert
        Job job;
        EXPECT_FALSE(heap.try_pop_and_push(job));
        for (bool is_present: present_ids) {
            EXPECT_TRUE(is_present);
        }
    }
}
