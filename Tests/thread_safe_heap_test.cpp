#include "gtest/gtest.h"
#include "../src/thread_safe_heap.h"

namespace thread_safe_heap_test{

    TEST(thread_safe_heap, single_thread_one_push){

        //arrange
        Job job{
                1,
                std::chrono::seconds(1),
                std::chrono::seconds(1),
                [](){}
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
}
