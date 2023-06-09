/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "ui/dialog/godot_export_dialog.hpp"
#include "ui/dialog/new_map_dialog.hpp"
#include "ui/dialog/resize_map_dialog.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/animation/animation_dock.hpp"
#include "ui/dock/comp/component_editor.hpp"
#include "ui/dock/editor/central_tileset_viewport.hpp"
#include "ui/dock/editor/editor_dock.hpp"
#include "ui/dock/layer/layer_dock.hpp"
#include "ui/dock/log/log_dock.hpp"
#include "ui/dock/property/property_dock.hpp"
#include "ui/dock/tileset/dialogs/new_tileset_dialog.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/dock/editor/central_map_viewport.hpp"

namespace tactile::ui {

struct WidgetState final {
  LayerDockState layer_dock;
  TilesetDockState tileset_dock;
  AnimationDockState animation_dock;
  EditorDockState editor_dock;
  CentralMapViewportState central_map_viewport;
  TilesetViewportState tileset_viewport;
  LogDockState log_dock;
  PropertyDockState property_dock;

  SettingsDialogState settings_dialog;
  ComponentEditorDialogState component_editor_dialog;
  NewMapDialogState new_map_dialog;
  NewTilesetDialogState new_tileset_dialog;
  ResizeMapDialogState resize_map_dialog;
  GodotExportDialogState godot_export_dialog;
};

}  // namespace tactile::ui
