#pragma once
#include <cstdint>
#include <istream>
#include <ostream>
class application;

enum class update_action : std::uint8_t {
  skip_render_next_frame,
  render_next_frame,
  exit,
};

// NOLINTBEGIN(cppcoreguidelines-special-member-functions)
// NOLINTBEGIN(hicpp-special-member-functions)
class abstract_page {
public:
  virtual ~abstract_page() = default;

  virtual void render(std::ostream &cout) = 0;
  virtual update_action update(application &app, std::istream &cin) = 0;
};
// NOLINTEND(hicpp-special-member-functions)
// NOLINTEND(cppcoreguidelines-special-member-functions)
