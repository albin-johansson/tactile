// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/shortcuts.hpp"

#include <imgui_internal.h>

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/events.hpp"

namespace tactile::core::ui {

void push_global_shortcuts(const Model& model, EventDispatcher& dispatcher)
{
  if (ImGui::Shortcut(kCreateMapShortcut.chord,
                      ImGuiKeyOwner_Any,
                      ImGuiInputFlags_RouteGlobal)) {
    dispatcher.push<ShowNewMapDialogEvent>();
  }

  if (ImGui::Shortcut(kOpenShortcut.chord, ImGuiKeyOwner_Any, ImGuiInputFlags_RouteGlobal)) {
    dispatcher.push<ShowOpenMapDialogEvent>();
  }

  if (ImGui::Shortcut(kOpenSettingsShortcut.chord,
                      ImGuiKeyOwner_Any,
                      ImGuiInputFlags_RouteGlobal)) {
    dispatcher.push<ShowSettingsDialogEvent>();
  }
}

}  // namespace tactile::core::ui
