#pragma once

namespace Tactile {

class LogDock final {
 public:
  void Update();

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  bool mHasFocus{};
};

}  // namespace Tactile
