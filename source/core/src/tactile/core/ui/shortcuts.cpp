// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/shortcuts.hpp"

#include <imgui_internal.h>

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/file_events.hpp"
#include "tactile/core/event/map_events.hpp"

namespace tactile::ui {

void push_global_shortcuts(const Model& model, EventDispatcher& dispatcher)
{
  if (ImGui::Shortcut(kCreateMapShortcut.chord,
                      ImGuiKeyOwner_Any,
                      ImGuiInputFlags_RouteGlobalLow)) {
    dispatcher.push<ShowNewMapDialogEvent>();
  }

  if (ImGui::Shortcut(kOpenMapShortcut.chord,
                      ImGuiKeyOwner_Any,
                      ImGuiInputFlags_RouteGlobalLow)) {
    dispatcher.push<ShowOpenMapDialogEvent>();
  }

  if (ImGui::Shortcut(kOpenSettingsShortcut.chord,
                      ImGuiKeyOwner_Any,
                      ImGuiInputFlags_RouteGlobalLow)) {
    dispatcher.push<ShowSettingsDialogEvent>();
  }
}

}  // namespace tactile::ui
