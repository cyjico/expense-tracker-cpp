#pragma once
// NOLINTBEGIN(cppcoreguidelines-special-member-functions)
// NOLINTBEGIN(hicpp-special-member-functions)
template <typename... Args> class observer {
public:
  virtual ~observer() = default;

  virtual void update(Args... args) = 0;
};
// NOLINTEND(hicpp-special-member-functions)
// NOLINTEND(cppcoreguidelines-special-member-functions)