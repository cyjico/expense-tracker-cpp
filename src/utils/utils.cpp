#include "utils/utils.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

bool utils::try_flush_failed_istream(std::istream &cin) {
  if (cin.fail()) {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
  }

  return false;
}

void utils::clear_screen(std::ostream &cout) {
  // For more info, see https://en.wikipedia.org/wiki/ANSI_escape_code
  // \x1B      => ASCII escape character in hexadecimal
  // \x1B[2J   => Clear entire screen (J) from top to bottom (2)
  // \x1B[1;1H => Position cursor at 1st row (1) and 1st column (1H)
  cout << "\x1B[2J\x1B[1;1H" << std::flush;
}

std::string utils::trim_string(const std::string &str) {
  const size_t first = str.find_first_not_of(" \t\n\r\f\v");
  if (first == std::string::npos) {
    return "";
  }

  const size_t last = str.find_last_not_of(" \t\n\r\f\v");

  return str.substr(first, last - first + 1);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std::string utils::double_to_string(const double &value, const int &precision,
                                    const bool &include_separator) {
  std::string str = std::to_string(value);

  size_t decimal_pos = str.rfind('.');
  if (decimal_pos != std::string::npos) {
    // remove trailing zeros/decimal point
    str.erase(str.find_last_not_of('0') + 1);

    if (str.back() != '.') {
      if (precision >= 0) {
        str.resize(decimal_pos + static_cast<size_t>(1 + precision));
      }
    } else {
      str.pop_back();
    }
  } else {
    decimal_pos = str.length();
  }

  if (include_separator && decimal_pos > 3) {
    for (int i = static_cast<int>(decimal_pos) - 3; i > 0; i -= 3) {
      str.insert(static_cast<size_t>(i), ",");
    }
  }

  return str;
}

namespace {

constexpr double winkler_scaling_factor = 0.1;

double winkler(const std::string &lhs, const std::string &rhs) {
  const size_t length =
      std::min({static_cast<size_t>(4ULL), lhs.length(), rhs.length()});
  int prefix_matches = 0;

  for (uint32_t i = 0; i < length; i++) {
    if (lhs[i] != rhs[i]) {
      break;
    }

    prefix_matches++;
  }

  return prefix_matches * winkler_scaling_factor;
}

} // namespace

// Implementation taken from
// https://www.geeksforgeeks.org/jaro-and-jaro-winkler-similarity/
double utils::jaro_winkler(const std::string &lhs, const std::string &rhs) {
  if (lhs == rhs) {
    return 1.0;
  }

  const auto lhs_len = static_cast<uint32_t>(lhs.length());
  const auto rhs_len = static_cast<uint32_t>(rhs.length());

  if (lhs_len == 0 || rhs_len == 0) {
    return 0;
  }

  const uint32_t max_dist = std::max(lhs_len, rhs_len) / 2 - 1;
  double matches = 0.0;

  std::vector<bool> lhs_hash(lhs_len, false);
  std::vector<bool> rhs_hash(rhs_len, false);
  for (uint32_t i = 0; i < lhs_len; i++) {
    for (uint32_t j = (i >= max_dist) ? (i - max_dist) : 0;
         j < std::min(rhs_len, i + max_dist + 1); j++) {

      if (lhs[i] == rhs[j] && !rhs_hash[j]) {
        lhs_hash[i] = rhs_hash[j] = true;
        matches++;
        break;
      }
    }
  }

  if (std::fabs(matches) < std::numeric_limits<double>::epsilon()) {
    return 0.0;
  }

  // Count transpositions
  uint32_t transpositions = 0;
  uint32_t point = 0;
  for (uint32_t i = 0; i < lhs_len; i++) {
    if (lhs_hash[i]) {
      while (!rhs_hash[point]) {
        point++;
      }

      if (lhs[i] != rhs[point]) {
        transpositions++;
      }
    }
  }

  transpositions /= 2;

  const double jaro_similarity = (matches / lhs_len + matches / rhs_len +
                                  (matches - transpositions) / matches) /
                                 3.0;

  return jaro_similarity + winkler(lhs, rhs) * (1 - jaro_similarity);
}
