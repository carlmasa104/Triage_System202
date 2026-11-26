#ifndef HISTORY_LOG_H
#define HISTORY_LOG_H

#include <string>

void logSystemEvent(const std::string& actor, const std::string& action, const std::string& details = "");
void viewHistoryLog();

#endif