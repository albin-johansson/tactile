#pragma once

#include <entt/entt.hpp>

#include "editor/gui/dialogs/credits_dialog.hpp"

namespace Tactile {

class Model;

class HelpMenu final {
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  void UpdateWindows();

 private:
  CreditsDialog mCreditsDialog;
  bool mShowAboutTactile{};
  bool mShowAboutImGui{};
};

}  // namespace Tactile
