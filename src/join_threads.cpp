#include <iostream>
#include "join_threads.h"

Join_threads::~Join_threads() {
    for (auto &thread: threads) {
        std::cout << "join_threads\n";
        if (thread.joinable())
            thread.join();
    }
}
