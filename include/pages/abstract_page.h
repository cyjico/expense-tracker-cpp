#pragma once
#include <cstdint>
#include <iostream>
class application;

enum class update_action : std::uint8_t {
  none = 0b1,
  exit_app = 0b10,
};

// NOLINTBEGIN(cppcoreguidelines-special-member-functions)
// NOLINTBEGIN(hicpp-special-member-functions)

/**
 * @brief Abstract class for "pages" in an application.
 */
class abstract_page {
public:
  virtual ~abstract_page() = default;

  /**
   * @brief Function that gets called every tick.
   *
   * For a guide on how to separate rendering and user input, it is reccomeneded
   * to render first before processing user input.
   *
   * @param app Reference to the application.
   * @param cout Reference to std::cout.
   * @param cin Reference to std::cin.
   * @return
   */
  virtual update_action update(application &app, std::ostream &cout,
                               std::istream &cin) = 0;
};
// NOLINTEND(hicpp-special-member-functions)
// NOLINTEND(cppcoreguidelines-special-member-functions)
