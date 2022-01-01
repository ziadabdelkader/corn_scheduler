include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

FetchContent_MakeAvailable(googletest)
include_directories("../plog-master/include")

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(Tests
        thread_safe_heap_test.cpp
        scheduler_test.cpp
        ../src/job.h
        ../src/job.cpp
        ../src/thread_safe_heap.cpp
        ../src/thread_safe_heap.h
        ../tasks.h
        ../tasks.cpp
        ../src/thread_pool.cpp
        ../src/thread_pool.h
        ../src/join_threads.h
        ../src/join_threads.cpp
        )

target_link_libraries(Tests gtest_main gmock)