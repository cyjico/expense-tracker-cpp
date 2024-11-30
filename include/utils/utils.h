#pragma once
#include <cstdint>
#include <istream>
#include <string>

namespace utils {

bool clear_failed_istream(std::istream &cin);

std::string trim_string(const std::string &str);

std::string double_to_string(const double &value, uint32_t precision = 2);

double jaro_winkler(const std::string &lhs, const std::string &rhs);

} // namespace utils
