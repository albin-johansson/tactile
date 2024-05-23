// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/layer_dock.hpp"

#include <imgui.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {

void LayerDock::update(const Language& language,
                       const MapDocument& document,
                       EventDispatcher& dispatcher)
{
  const Window dock_window {language.get(StringID::kLayerDock)};
  if (dock_window.is_open()) {
  }
}

}  // namespace tactile::ui
