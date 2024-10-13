// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/i18n/language.hpp"

#include <utility>  // to_underlying

#include "tactile/core/ui/common/icons.hpp"

namespace tactile::core::ui {

// clang-format off
static_assert(std::to_underlying(MiscLabel::kNEXT) == std::to_underlying(NounLabel::kMIN));
static_assert(std::to_underlying(NounLabel::kNEXT) == std::to_underlying(VerbLabel::kMIN));
static_assert(std::to_underlying(VerbLabel::kNEXT) == std::to_underlying(AdjectiveLabel::kMIN));
static_assert(std::to_underlying(AdjectiveLabel::kNEXT) == std::to_underlying(ActionLabel::kMIN));
static_assert(std::to_underlying(ActionLabel::kNEXT) == std::to_underlying(HintLabel::kMIN));
// clang-format on

Language::Language(const LanguageID id, std::vector<std::string> strings)
  : mID {id},
    mStrings {std::move(strings)}
{}

void Language::add_icons()
{
  auto inject_icon = [this](const auto string_id, const char* icon) {
    auto& string = mStrings[std::to_underlying(string_id)];
    string.insert(0, icon);
  };

  // File
  inject_icon(NounLabel::kRecentFilesMenu, to_prefix_string(Icon::kHistory));
  inject_icon(ActionLabel::kClearFileHistory, to_prefix_string(Icon::kGarbage));
  inject_icon(ActionLabel::kCreateMap, to_prefix_string(Icon::kNewFile));
  inject_icon(ActionLabel::kOpen, to_prefix_string(Icon::kOpen));
  inject_icon(ActionLabel::kOpenMap, to_prefix_string(Icon::kOpen));
  inject_icon(ActionLabel::kOpenTileset, to_prefix_string(Icon::kOpen));
  inject_icon(VerbLabel::kSave, to_prefix_string(Icon::kSave));
  inject_icon(ActionLabel::kSaveAs, to_prefix_string(Icon::kSaveAs));
  inject_icon(VerbLabel::kClose, to_prefix_string(Icon::kCloseFile));
  inject_icon(VerbLabel::kQuit, to_prefix_string(Icon::kQuit));

  // Edit
  inject_icon(VerbLabel::kUndo, to_prefix_string(Icon::kUndo));
  inject_icon(VerbLabel::kRedo, to_prefix_string(Icon::kRedo));
  inject_icon(ActionLabel::kStampTool, to_prefix_string(Icon::kStamp));
  inject_icon(ActionLabel::kEraserTool, to_prefix_string(Icon::kEraser));
  inject_icon(ActionLabel::kBucketTool, to_prefix_string(Icon::kBucket));
  inject_icon(ActionLabel::kObjectSelectorTool, to_prefix_string(Icon::kSelect));
  inject_icon(ActionLabel::kRectangleTool, to_prefix_string(Icon::kRectangle));
  inject_icon(ActionLabel::kEllipseTool, to_prefix_string(Icon::kEllipse));
  inject_icon(ActionLabel::kPointTool, to_prefix_string(Icon::kPoint));
  inject_icon(ActionLabel::kOpenComponentEditor, to_prefix_string(Icon::kComponent));
  inject_icon(ActionLabel::kOpenSettings, to_prefix_string(Icon::kSettings));

  // View
  inject_icon(NounLabel::kWidgetsMenu, to_prefix_string(Icon::kWidget));
  inject_icon(NounLabel::kThemeMenu, to_prefix_string(Icon::kTheme));
  inject_icon(ActionLabel::kPanUp, to_prefix_string(Icon::kMoveUp));
  inject_icon(ActionLabel::kPanDown, to_prefix_string(Icon::kMoveDown));
  inject_icon(ActionLabel::kPanLeft, to_prefix_string(Icon::kMoveLeft));
  inject_icon(ActionLabel::kPanRight, to_prefix_string(Icon::kMoveRight));
  inject_icon(ActionLabel::kCenterViewport, to_prefix_string(Icon::kAlignCenter));
  inject_icon(ActionLabel::kIncreaseZoom, to_prefix_string(Icon::kZoomIn));
  inject_icon(ActionLabel::kDecreaseZoom, to_prefix_string(Icon::kZoomOut));
  inject_icon(ActionLabel::kResetZoom, to_prefix_string(Icon::kZoomReset));
  inject_icon(ActionLabel::kToggleGrid, to_prefix_string(Icon::kGrid));
  inject_icon(ActionLabel::kToggleUi, to_prefix_string(Icon::kEye));

  // Map
  inject_icon(ActionLabel::kAddTileset, to_prefix_string(Icon::kImage));
  inject_icon(ActionLabel::kResize, to_prefix_string(Icon::kResize));
  inject_icon(ActionLabel::kFixInvalidTiles, to_prefix_string(Icon::kRepair));
  inject_icon(NounLabel::kExportAsMenu, to_prefix_string(Icon::kExport));

  // Help
  inject_icon(ActionLabel::kAboutTactile, to_prefix_string(Icon::kQuestion));
  inject_icon(ActionLabel::kReportBug, to_prefix_string(Icon::kBug));
  inject_icon(ActionLabel::kOpenCredits, to_prefix_string(Icon::kCode));

  // Debug
  inject_icon(ActionLabel::kOpenDebugger, to_prefix_string(Icon::kDebug));
  inject_icon(ActionLabel::kOpenStyleEditor, to_prefix_string(Icon::kBrush));
  inject_icon(ActionLabel::kOpenDemoWindow, to_prefix_string(Icon::kToolbox));
  inject_icon(ActionLabel::kOpenStorageDir, to_prefix_string(Icon::kOpen));

  inject_icon(ActionLabel::kCreateProperty, to_prefix_string(Icon::kCreate));
  inject_icon(ActionLabel::kRemoveProperty, to_prefix_string(Icon::kRemove));
  inject_icon(ActionLabel::kRenameProperty, to_prefix_string(Icon::kEdit));

  inject_icon(ActionLabel::kCreateTileset, to_prefix_string(Icon::kCreate));
  inject_icon(ActionLabel::kCreateLayer, to_prefix_string(Icon::kCreate));

  inject_icon(ActionLabel::kShowMetadata, to_prefix_string(Icon::kInfo));
  inject_icon(ActionLabel::kSelectImage, to_prefix_string(Icon::kImage));
  inject_icon(ActionLabel::kTileLayerItem, to_prefix_string(Icon::kTileLayer));
  inject_icon(ActionLabel::kObjectLayerItem, to_prefix_string(Icon::kObjectLayer));
  inject_icon(ActionLabel::kGroupLayerItem, to_prefix_string(Icon::kGroupLayer));
}

auto Language::get_id() const -> LanguageID
{
  return mID;
}

}  // namespace tactile::core::ui
