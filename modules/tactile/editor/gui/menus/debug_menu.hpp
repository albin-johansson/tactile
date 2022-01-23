#pragma once

namespace tactile {

class DebugMenu final {
 public:
  void Update();

  void UpdateWindows();

 private:
  bool mShowMetrics{};
  bool mShowDemo{};
  bool mShowStyleEditor{};
};

}  // namespace tactile
