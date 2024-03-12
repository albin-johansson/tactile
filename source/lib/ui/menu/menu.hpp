// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

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
