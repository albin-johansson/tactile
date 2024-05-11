// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/// \addtogroup UI
/// \{

/**
 * Provides identifiers for all user-facing strings.
 *
 * \details
 * The enumerators are effectively indices into in an array of translated
 * strings, which allows for very fast string lookups at runtime. For this
 * reason, it's important to keep the underlying range of values as small as
 * possible to minimize memory usage. Basically, don't provide explicit
 * enumerator values.
 */
enum class StringID : usize  // NOLINT(*-enum-size)
{
  // Miscellaneous.
  kOK,

  // Generic nouns.
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
  kRow,
  kRows,
  kColumn,
  kColumns,
  kOrientation,
  kTileWidth,
  kTileHeight,

  // Generic verbs.
  kCancel,
  kApply,
  kClose,
  kCreate,
  kUndo,
  kRedo,
  kSave,
  kQuit,

  // Generic adjectives.
  kOrthogonal,
  kHexagonal,

  // Window names.
  kLayerDock,
  kTilesetDock,
  kPropertyDock,
  kComponentDock,
  kAnimationDock,
  kLogDock,
  kStyleEditorWidget,

  // Menu names.
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

  // "Action" strings. These often have embedded leading icons.
  kCreateMap,
  kOpenMap,
  kOpenTileset,
  kOpenComponentEditor,
  kOpenSettings,
  kSaveAs,
  kIncreaseFontSize,
  kDecreaseFontSize,
  kResetFontSize,
  kIncreaseZoom,
  kDecreaseZoom,
  kResetZoom,
  kCenterViewport,
  kToggleGrid,
  kPanUp,
  kPanDown,
  kPanLeft,
  kPanRight,
  kHighlightActiveLayer,
  kToggleUi,
  kStampTool,
  kEraserTool,
  kBucketTool,
  kObjectSelectorTool,
  kRectangleTool,
  kEllipseTool,
  kPointTool,
  kReportBug,
  kOpenDemoWindow,
  kOpenStyleEditor,

  /** The number of string identifiers, not a string itself. */
  kMAX
};

/// \}

}  // namespace tactile::ui
