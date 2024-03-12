// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

namespace tactile {

/// Tracks visibility of widgets for the "Toggle UI" feature.
struct WidgetShowState final {
  bool prev_show_layer_dock     : 1 {};
  bool prev_show_tileset_dock   : 1 {};
  bool prev_show_property_dock  : 1 {};
  bool prev_show_log_dock       : 1 {};
  bool prev_show_component_dock : 1 {};
  bool prev_show_animation_dock : 1 {};
};

}  // namespace tactile
