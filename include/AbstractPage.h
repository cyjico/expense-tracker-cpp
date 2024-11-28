#pragma once
#include <cstdint>
#include <istream>
class Application;

enum class UpdateAction : std::uint8_t {
  NONE,
  RENDER,
  EXIT,
};

// NOLINTBEGIN(cppcoreguidelines-special-member-functions)
// NOLINTBEGIN(hicpp-special-member-functions)
class AbstractPage {
public:
  virtual ~AbstractPage() = default;

  virtual void Render(std::ostream &cout) = 0;
  virtual UpdateAction Update(Application &app, std::istream &cin) = 0;
};
// NOLINTEND(hicpp-special-member-functions)
// NOLINTEND(cppcoreguidelines-special-member-functions)
