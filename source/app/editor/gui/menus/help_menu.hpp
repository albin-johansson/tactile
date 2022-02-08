#pragma once

#include <entt/entt.hpp>

#include "editor/gui/dialogs/about_dialog.hpp"
#include "editor/gui/dialogs/credits_dialog.hpp"

namespace tactile {

class Model;

class HelpMenu final
{
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  void UpdateWindows();

 private:
  about_dialog mAboutDialog;
  CreditsDialog mCreditsDialog;
  bool mShowAboutImGui{};
};

}  // namespace tactile
