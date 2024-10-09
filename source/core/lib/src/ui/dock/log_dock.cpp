// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/log_dock.hpp"

#include <imgui.h>

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::core::ui {

void LogDock::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const Window dock_window {language.get(StringID::kLogDock)};
  if (dock_window.is_open()) {
  }
}

}  // namespace tactile::core::ui
