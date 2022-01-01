# Cron scheduler

## README

Description of the solution:

- Scheduler is implemented as a thread pool.
- Whenever it's time to execute some job, it is assigned to some thread from the pool to run it.
- Jobs are maintained in a heap where jobs with sooner times are given higher priority.
- Threads in the pool check the head of the heap regularly. If it is time to execute the head, it gets executed and the same job is added back to the heap with the next scheduled time.

Technical decisions:

- Thread pool is used to optimize performance as creating and deleting a thread for each job run is expensive.
- A heap is used to access the soonest job and schedule jobs effectively.

Trade-offs:
- When there is no job to run, threads in the pool block only for a short time rather than blocking until it is time to run the soonest job.This is to handle the case when a new job arrives with time sooner than all other jobs in the heap.

Improvements:

- Better dynamic estimation for the number of threads in the pool.
- Implement thread interrupt to be able to wake up a thread when a new job arrives with time sooner than all other jobs in the heap.

Usage:
- Jobs are submitted to the scheduler with:
  - Job id.
  - Scheduling frequency.
  - Expected time of single run.
  - A function implementation.
```
Thread_pool scheduler{};

std::vector<Job> jobs{
        Job(
                1, //job id
                std::chrono::seconds (10), // Scheduling frequency
                std::chrono::milliseconds (100), //Expected time of a single run
                []() { write_many("job1.txt", 1e5); } //function to run
        ),
        Job(
                2,
                std::chrono::seconds (20),
                std::chrono::milliseconds (2000),
                []() { mod_many("job2.txt", 1e8); }
        ),
        Job(
                3,
                std::chrono::seconds (5),
                std::chrono::milliseconds (5),
                []() { write_to_file("job3.txt", "hello world\n"); }
        ),
};

for(const auto& job: jobs){
    scheduler.submit(job);
}
```
- Log file of running the example above:
```
2022-01-01 13:13:59.189 INFO  [8452] [Job::execute@6] Job 1 starts
2022-01-01 13:13:59.189 INFO  [8450] [Job::execute@6] Job 2 starts
2022-01-01 13:13:59.189 INFO  [8451] [Job::execute@6] Job 3 starts
2022-01-01 13:13:59.193 INFO  [8451] [Job::execute@15] Job 3 ends. Actual execution time = 0 milliseconds. Expected execution time = 5 milliseconds.
2022-01-01 13:13:59.232 INFO  [8452] [Job::execute@15] Job 1 ends. Actual execution time = 41 milliseconds. Expected execution time = 100 milliseconds.
2022-01-01 13:14:04.189 INFO  [8452] [Job::execute@6] Job 3 starts
2022-01-01 13:14:04.190 INFO  [8452] [Job::execute@15] Job 3 ends. Actual execution time = 0 milliseconds. Expected execution time = 5 milliseconds.
2022-01-01 13:14:05.077 INFO  [8450] [Job::execute@15] Job 2 ends. Actual execution time = 5886 milliseconds. Expected execution time = 2000 milliseconds.
2022-01-01 13:14:09.189 INFO  [8452] [Job::execute@6] Job 1 starts
2022-01-01 13:14:09.189 INFO  [8450] [Job::execute@6] Job 3 starts
2022-01-01 13:14:09.190 INFO  [8450] [Job::execute@15] Job 3 ends. Actual execution time = 0 milliseconds. Expected execution time = 5 milliseconds.
2022-01-01 13:14:09.232 INFO  [8452] [Job::execute@15] Job 1 ends. Actual execution time = 42 milliseconds. Expected execution time = 100 milliseconds.
2022-01-01 13:14:14.189 INFO  [8451] [Job::execute@6] Job 3 starts
2022-01-01 13:14:14.190 INFO  [8451] [Job::execute@15] Job 3 ends. Actual execution time = 0 milliseconds. Expected execution time = 5 milliseconds.
2022-01-01 13:14:19.189 INFO  [8452] [Job::execute@6] Job 1 starts
2022-01-01 13:14:19.190 INFO  [8453] [Job::execute@6] Job 3 starts
2022-01-01 13:14:19.190 INFO  [8453] [Job::execute@15] Job 3 ends. Actual execution time = 0 milliseconds. Expected execution time = 5 milliseconds.
2022-01-01 13:14:19.189 INFO  [8451] [Job::execute@6] Job 2 starts
2022-01-01 13:14:19.229 INFO  [8452] [Job::execute@15] Job 1 ends. Actual execution time = 39 milliseconds. Expected execution time = 100 milliseconds.
2022-01-01 13:14:24.189 INFO  [8450] [Job::execute@6] Job 3 starts
2022-01-01 13:14:24.190 INFO  [8450] [Job::execute@15] Job 3 ends. Actual execution time = 0 milliseconds. Expected execution time = 5 milliseconds.
2022-01-01 13:14:25.684 INFO  [8451] [Job::execute@15] Job 2 ends. Actual execution time = 6493 milliseconds. Expected execution time = 2000 milliseconds.
```

References:
- Anthony Williams, E 2012, *C++ Concurrency in Action Practical Multithreading*.