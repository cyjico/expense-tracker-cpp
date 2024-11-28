#pragma once
#include <istream>
#include <string>

namespace util {

bool clear_failed_istream(std::istream &cin);

std::string trim_string(const std::string &str);

} // namespace util