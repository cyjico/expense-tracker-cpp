#include "utils/utils.h"
#include <cstddef>
#include <iostream>
#include <limits>
#include <string>

bool utils::clear_failed_istream(std::istream &cin) {
  if (cin.fail()) {
    // Clear error flag
    cin.clear();
    // Skip to the next newline
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
  }

  return false;
}

std::string utils::trim_string(const std::string &str) {
  const size_t first = str.find_first_not_of(" \t\n\r\f\v");
  if (first == std::string::npos) {
    return "";
  }

  const size_t last = str.find_last_not_of(" \t\n\r\f\v");

  return str.substr(first, last - first + 1);
}

std::string utils::double_to_string(const double &value) {
  std::string result = std::to_string(value);

  result.erase(result.find_last_not_of('0') + 1, std::string::npos);
  if (result.back() == '.') {
    result.pop_back();
  }

  return result;
}
