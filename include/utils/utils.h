#pragma once
#include <cstdint>
#include <iostream>
#include <string>

namespace utils {

/**
 * @brief Clears/flushes error flags and any leftover input of a failed istream.
 *
 * @param cin
 * @return Whether it has successfully cleared the error flags and any leftover
 * input or not.
 */
bool try_flush_failed_istream(std::istream &cin);
void clear_screen(std::ostream &cout);

std::string trim_string(const std::string &str);
std::string double_to_string(const double &value, uint32_t precision = 2);

double jaro_winkler(const std::string &lhs, const std::string &rhs);

} // namespace utils
