// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/menu/help_menu.hpp"

#include <SDL2/SDL.h>
#include <imgui.h>

#include "tactile/core/event/dialog_events.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/menus.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {
inline namespace help_menu {

inline constexpr const char* kReportBugUrl =
    "https://github.com/albin-johansson/tactile/issues/new";

}  // namespace help_menu

void HelpMenu::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();

  const MenuScope menu {language.get(StringID::kHelpMenu)};
  if (menu.is_open()) {
    if (ImGui::MenuItem(language.get(StringID::kAboutTactile))) {
      dispatcher.push<ShowAboutDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kReportBug))) {
      if (SDL_OpenURL(kReportBugUrl) != 0) {
        TACTILE_LOG_ERROR("Could not open issue URL: {}", SDL_GetError());
      }
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kOpenCredits))) {
      dispatcher.push<ShowCreditsDialogEvent>();
    }
  }
}

}  // namespace tactile::ui
