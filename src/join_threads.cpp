#include "join_threads.h"

Join_threads::~Join_threads() {
    for (auto &thread: threads) {
        if (thread.joinable())
            thread.join();
    }
}
