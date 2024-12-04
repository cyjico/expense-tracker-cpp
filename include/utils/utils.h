#pragma once
#include <iostream>
#include <string>

namespace utils {

/**
 * @brief Clears/flushes error flags and any leftover input of a failed istream.
 *
 * @param cin Input stream to flush.
 * @return Whether it has successfully cleared the error flags and any leftover
 * input or not.
 */
bool try_flush_failed_istream(std::istream &cin);
void clear_screen(std::ostream &cout);

std::string trim_string(const std::string &str);
std::string double_to_string(const double &value, const int &precision = 2,
                             const bool &include_separator = true);

/**
 * @brief Computes the Jaro-Winkler similarity between two strings.
 *
 * It is a metric that measures the similarity between two strings, with a bias
 * to strings that match at the beginning.
 *
 * @param lhs First string to compare for.
 * @param rhs Second string to compare against.
 * @return Similarity score between 0 and 1, where 1 is an exact match.
 */
double jaro_winkler(const std::string &lhs, const std::string &rhs);

} // namespace utils
