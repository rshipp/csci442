#pragma once
#include <string>


std::string humanize_bytes(long bytes);

std::string humanize_time(int ticks);

std::string humanize_uptime(float seconds);

std::string to_string(long num);
