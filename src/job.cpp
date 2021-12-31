#include "job.h"
#include "plog/Log.h"


void Job::execute() {
    PLOGI << "Job " << id << " starts";
    auto start = std::chrono::system_clock::now();
    try {
        fun();
    } catch (...) {
        PLOGI << "Job " << id << " failed.";
        return;
    }
    auto end = std::chrono::system_clock::now();
    PLOGI << "Job " << id << " ends."
          << " Actual execution time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
          << " milliseconds." << " Expected execution time = " << expected_interval.count() << " milliseconds.";
}
