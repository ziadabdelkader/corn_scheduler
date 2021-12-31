#ifndef CRON_SCHEDULER_TASKS_H
#define CRON_SCHEDULER_TASKS_H
#include <string>

// example tasks for testing

void write_to_file(const std::string& path, const std::string& message);
void write_many(const std::string& path, int32_t count);
void mod_many(const std::string& path, int32_t count);
void failing();
#endif //CRON_SCHEDULER_TASKS_H
