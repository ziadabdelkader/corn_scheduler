cmake_minimum_required(VERSION 3.21)
project(corn_scheduler_1)
add_subdirectory(Tests)
include_directories("./plog-master/include")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(corn_scheduler_1 main.cpp src/job.cpp src/job.h src/join_threads.cpp src/join_threads.h src/thread_pool.cpp src/thread_pool.h src/thread_safe_heap.cpp src/thread_safe_heap.h tasks.cpp tasks.h)
