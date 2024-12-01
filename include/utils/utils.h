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
std::string double_to_string(const double &value, const uint32_t &precision = 2,
                             const bool &has_separator = true);

/**
 * @brief Computes the Jaro-Winkler similarity between two strings.
 *
 * It is a metric that measures the similarity between two strings, with a bias
 * to strings that match at the beginning.
 *
 * @param lhs
 * @param rhs
 * @return Similarity score between 0 and 1, where 1 is an exact match.
 */
double jaro_winkler(const std::string &lhs, const std::string &rhs);

} // namespace utils
