// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/layer_dock_widget.hpp"

#include <imgui.h>

#include "tactile/core/model/model.hpp"

namespace tactile::core {

void LayerDockWidget::update(const Model& model, EventDispatcher&)
{
  const auto& settings = model.settings();

  if (settings.show_tileset_dock) {
    return;
  }

  // TODO
}

}  // namespace tactile::core
