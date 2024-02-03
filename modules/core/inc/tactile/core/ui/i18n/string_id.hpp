// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * Provides identifiers for all user-facing strings.
 *
 * \note
 *    The enumerators are intended to be used as indices into in an array
 *    of translated strings, and MUST be sequential.
 */
enum class StringID : usize {  // NOLINT(*-enum-size)
  // Generic
  kOK,
  kCancel,
  kApply,
  kClose,
  kCreate,

  // Widgets
  kLayerDock,
  kTilesetDock,
  kPropertyDock,
  kComponentDock,
  kAnimationDock,
  kLogDock,
  kStyleEditorWidget,

  // Menus
  kFileMenu,
  kEditMenu,
  kViewMenu,
  kMapMenu,
  kTilesetMenu,
  kHelpMenu,
  kDebugMenu,
  kRecentFilesMenu,
  kWidgetsMenu,
  kExportAsMenu,

  // Actions
  kUndo,
  kRedo,
  kCreateMap,
  kOpenMap,
  kOpenTileset,
  kSave,
  kSaveAs,
  kCenterViewport,
  kIncreaseZoom,
  kDecreaseZoom,
  kResetZoom,
  kToggleGrid,
  kPanUp,
  kPanDown,
  kPanLeft,
  kPanRight,
  kReportBug,
  kOpenDemoWindow,
  kOpenStyleEditor,

  // Types
  kInt,
  kInt2,
  kInt3,
  kInt4,
  kFloat,
  kFloat2,
  kFloat3,
  kFloat4,
  kString,
  kColor,
  kObject,
  kPath,
  kBool,

  kMAX  ///< The number of string identifiers, not a string itself.
};

}  // namespace tactile::core
