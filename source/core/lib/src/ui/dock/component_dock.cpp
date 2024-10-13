// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/component_dock.hpp"

#include <imgui.h>

#include "tactile/base/document/document.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::core::ui {

void ComponentDock::push(const Language& language,
                         const IDocument& document,
                         EventDispatcher& dispatcher)
{
  const Window dock_window {language.get(NounLabel::kComponentDock)};
  if (dock_window.is_open()) {
  }
}

}  // namespace tactile::core::ui
