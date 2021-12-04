#pragma once

namespace Tactile {

/// \addtogroup gui
/// \{

class HelpMenu final {
 public:
  void Update();

  void UpdateWindows();

 private:
  bool mShowAboutTactile{};
  bool mShowAboutImGui{};
  bool mShowCredits{};
};

/// \} End of group gui

}  // namespace Tactile
