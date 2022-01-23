#pragma once

#include "logging.hpp"

namespace tactile {

class LogDock final {
 public:
  void Update();

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  LogLevel mLogLevel{LogLevel::Verbose};
  bool mHasFocus{};
};

}  // namespace tactile
