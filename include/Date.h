#pragma once
#include <cstdint>
#include <string>

struct Date {
  uint8_t day;
  uint8_t month;
  uint16_t year;

  Date(uint8_t day, uint8_t month, uint16_t year);

  std::string ToString() const;
  bool IsValid() const;
};
