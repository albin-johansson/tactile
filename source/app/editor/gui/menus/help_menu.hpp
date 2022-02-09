#pragma once

#include <entt/entt.hpp>

#include "editor/gui/dialogs/about_dialog.hpp"
#include "editor/gui/dialogs/credits_dialog.hpp"

namespace tactile {

class document_model;

class HelpMenu final
{
 public:
  void Update(const document_model& model, entt::dispatcher& dispatcher);

  void UpdateWindows();

 private:
  about_dialog mAboutDialog;
  credits_dialog mCreditsDialog;
  bool mShowAboutImGui{};
};

}  // namespace tactile
