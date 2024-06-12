// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/document_dock.hpp"

#include <imgui.h>

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {

void DocumentDock::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const Window dock_window {language.get(StringID::kDocumentDock),
                            ImGuiWindowFlags_NoScrollbar};
  if (dock_window.is_open()) {
  }
}

}  // namespace tactile::ui
