#include "utils/utils.h"
#include <algorithm>
#include <cstddef>
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
        str.resize(decimal_pos + (1 + precision));
      }
    } else {
      str.pop_back();
    }
  } else {
    decimal_pos = str.length();
  }

  if (include_separator && decimal_pos > 3) {
    for (int i = static_cast<int>(decimal_pos) - 3; i > 0; i -= 3) {
      str.insert(i, ",");
    }
  }

  return str;
}

// Implementation taken from
// https://www.geeksforgeeks.org/jaro-and-jaro-winkler-similarity/
double utils::jaro_winkler(const std::string &lhs, const std::string &rhs) {
  if (lhs == rhs) {
    return 1.0;
  }

  const int lhs_len = static_cast<int>(lhs.length());
  const int rhs_len = static_cast<int>(rhs.length());

  if (lhs_len == 0) {
    return rhs_len == 0 ? 1.0 : 0.0;
  }

  const int max_dist = std::max(lhs_len, rhs_len) / 2 - 1;
  int matches = 0;

  std::vector<bool> lhs_hash(lhs_len, false);
  std::vector<bool> rhs_hash(rhs_len, false);
  for (int i = 0; i < lhs_len; i++) {
    for (int j = std::max(0, i - max_dist);
         j < std::min(rhs_len, i + max_dist + 1); j++) {

      if (lhs[i] == rhs[j] && !rhs_hash[j]) {
        lhs_hash[i] = rhs_hash[j] = true;
        matches++;
        break;
      }
    }
  }

  if (matches == 0) {
    return 0.0;
  }

  // Count transpositions
  int transpositions = 0;
  int point = 0;
  for (int i = 0; i < lhs_len; i++) {
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

  const double jaro_similarity =
      (matches / static_cast<double>(lhs_len) +
       matches / static_cast<double>(rhs_len) +
       (matches - transpositions) / static_cast<double>(matches)) /
      3.0;

  // Winkler (bias in prefix similarity)
  constexpr double winkler_scaling_factor = 0.1;
  int prefix_length = 0;
  for (int i = 0; i < std::min({4, lhs_len, rhs_len}); i++) {
    if (lhs[i] == rhs[i]) {
      prefix_length++;
    } else {
      break;
    }
  }

  return jaro_similarity +
         prefix_length * winkler_scaling_factor * (1 - jaro_similarity);
}
