#pragma once

namespace Tactile {

class DebugMenu final {
 public:
  void Update();

  void UpdateWindows();

 private:
  bool mShowMetrics{};
  bool mShowDemo{};
  bool mShowStyleEditor{};
};

}  // namespace Tactile
