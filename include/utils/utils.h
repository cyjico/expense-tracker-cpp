#pragma once
#include <istream>
#include <string>

namespace utils {

bool clear_failed_istream(std::istream &cin);

std::string trim_string(const std::string &str);

} // namespace utils
