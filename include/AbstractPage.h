#pragma once
#include <cstdint>
#include <istream>
class Application;

enum class UpdateAction : std::uint8_t {
  NONE,
  RENDER,
  EXIT,
};

class AbstractPage {
public:
  virtual ~AbstractPage() = default;

  virtual void Render(std::ostream &cout) = 0;
  virtual UpdateAction Update(Application &app, std::istream &cin) = 0;
};
