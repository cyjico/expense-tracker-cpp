#pragma once
#include <istream>
#include <string>

namespace util {

bool ClearFailedIstream(std::istream &cin);

std::string TrimString(const std::string &str);

} // namespace util