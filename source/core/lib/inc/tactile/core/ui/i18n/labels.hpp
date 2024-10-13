// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>  // size_t
#include <utility>  // to_underlying

namespace tactile::core::ui {

/* These enumerators act as indices into in an array of translated strings, which allows for
   very fast string lookups. For this reason, it's vital the enumerators of the different enums
   together form a contiguous sequence of indices. This is achieved by adding "kMIN" and
   "kNEXT" enumerators to each enum, which encode the value of the first enumerator and the
   next available index, respectively. Each enum sets their "kMIN" enumerator to the value of
   the "kNEXT" enumerator from the enum directly above itself. The range of an enum E is then
   [E::kMIN, E::kNEXT). */

enum class MiscLabel : std::size_t
{
  kMIN = 0,

  kOK = kMIN,

  kNEXT,
};

enum class NounLabel : std::size_t
{
  kMIN = std::to_underlying(MiscLabel::kNEXT),

  kInt = kMIN,
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
  kName,
  kType,
  kValue,
  kTileWidth,
  kTileHeight,
  kLightThemes,
  kDarkThemes,
  kFont,
  kDefault,
  kVersion,
  kProjectDir,

  // Window names.
  kDocumentDock,
  kLayerDock,
  kTilesetDock,
  kPropertyDock,
  kComponentDock,
  kAnimationDock,
  kLogDock,
  kStyleEditorWidget,
  kGodotExportDialog,

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
  kThemeMenu,
  kExportAsMenu,

  kNEXT,
};

enum class VerbLabel : std::size_t
{
  kMIN = std::to_underlying(NounLabel::kNEXT),

  kCancel = kMIN,
  kApply,
  kClose,
  kCreate,
  kRename,
  kUndo,
  kRedo,
  kSave,
  kQuit,

  kNEXT,
};

enum class AdjectiveLabel : std::size_t
{
  kMIN = std::to_underlying(VerbLabel::kNEXT),

  kOrthogonal = kMIN,
  kHexagonal,

  kNEXT,
};

enum class ActionLabel : std::size_t
{
  kMIN = std::to_underlying(AdjectiveLabel::kNEXT),

  kCreateMap = kMIN,
  kCreateTileset,
  kCreateLayer,
  kOpen,
  kOpenMap,
  kOpenTileset,
  kOpenComponentEditor,
  kOpenSettings,
  kSaveAs,
  kReopenLastClosedFile,
  kClearFileHistory,
  kResetLayout,
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
  kAddTileset,
  kAddRow,
  kAddColumn,
  kRemoveRow,
  kRemoveColumn,
  kResize,
  kFixInvalidTiles,
  kCreateProperty,
  kRemoveProperty,
  kRenameProperty,
  kChangePropertyType,
  kSelectImage,
  kShowMetadata,
  kReportBug,
  kAboutTactile,
  kOpenCredits,
  kOpenDebugger,
  kOpenStyleEditor,
  kOpenDemoWindow,
  kOpenStorageDir,
  kTileLayerItem,
  kObjectLayerItem,
  kGroupLayerItem,

  kNEXT,
};

enum class HintLabel : std::size_t
{
  kMIN = std::to_underlying(ActionLabel::kNEXT),

  kContextHasNoProperties = kMIN,
  kContextHasNoComponents,
  kMapHasNoLayers,
  kMapHasNoTilesets,
  kSelectTilesetImage,

  kNEXT,
};

inline constexpr std::size_t kLabelCount = std::to_underlying(HintLabel::kNEXT);

}  // namespace tactile::core::ui
