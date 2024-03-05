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

#include <SDL.h>

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(Strings)
TACTILE_FWD_DECLARE_CLASS(DocumentModel)

/// Encodes all possible menu actions, corresponding to various menu items.
enum class MenuAction {
  // File
  NewMap = SDL_USEREVENT + 1,
  OpenMap,
  Save,
  SaveAs,
  CloseDocument,
  Quit,
  ReopenLastClosedFile,
  ClearFileHistory,

  // Edit
  Undo,
  Redo,
  StampTool,
  EraserTool,
  BucketTool,
  ObjectSelectionTool,
  RectangleTool,
  EllipseTool,
  PointTool,
  ComponentEditor,
  OpenSettings,

  // View
  CenterViewport,
  ToggleGrid,
  IncreaseZoom,
  DecreaseZoom,
  ResetZoom,
  IncreaseFontSize,
  DecreaseFontSize,
  ResetFontSize,
  PanUp,
  PanDown,
  PanRight,
  PanLeft,
  HighlightLayer,
  ToggleUi,

  // Map
  InspectMap,
  AddTileset,
  AddRow,
  AddColumn,
  RemoveRow,
  RemoveColumn,
  FixInvalidTiles,
  ResizeMap,
  ExportGodotScene,

  // Tileset
  InspectTileset,

  // Debug
  DemoWindow,
  StyleEditor,

  // Help
  AboutTactile,
  AboutDearImGui,
  ReportIssue,
  Credits
};

/// Provides data for a single menu item entry.
struct MenuItem final {
  String text;                ///< The menu item label, in the current language.
  bool enabled  : 1 {true};   ///< Is the menu item possible to activate?
  bool selected : 1 {false};  ///< Should the menu item be rendered with a checkmark?
};

/// Initializes the necessary in-memory menu state.
/// This function should only be called once.
void init_menus();

/// Refreshes the state of all menu item entries.
void update_menus(const DocumentModel& model);

/// Translates all menu labels to another language.
void menu_translate(const Strings& strings);

/// Sets whether a specific menu action is available.
void menu_set_enabled(MenuAction action, bool enabled);

/// Sets whether a specific menu action features an associated checkmark.
void menu_set_selected(MenuAction action, bool selected);

/// Returns the data associated with the menu item for an action.
[[nodiscard]] auto get_menu_item(MenuAction action) -> const MenuItem&;

}  // namespace tactile
