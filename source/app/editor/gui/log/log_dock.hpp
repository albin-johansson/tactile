#pragma once

#include "misc/logging.hpp"

namespace tactile {

class LogDock final {
 public:
  void Update();

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  log_level mLogLevel{log_level::verbose};
  bool mHasFocus{};
};

}  // namespace tactile
