// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/i18n/language.hpp"

#include "tactile/core/ui/common/icons.hpp"

namespace tactile::ui {

Language::Language(const LanguageID id, std::vector<String> strings) :
  mID {id},
  mStrings {std::move(strings)}
{}

void Language::add_icons()
{
  auto inject_icon = [this](const StringID string_id, const char* icon) {
    auto& string = mStrings[std::to_underlying(string_id)];
    string.insert(0, icon);
  };

  // File
  inject_icon(StringID::kRecentFilesMenu, to_prefix_string(Icon::kHistory));
  inject_icon(StringID::kClearFileHistory, to_prefix_string(Icon::kGarbage));
  inject_icon(StringID::kCreateMap, to_prefix_string(Icon::kNewFile));
  inject_icon(StringID::kOpen, to_prefix_string(Icon::kOpen));
  inject_icon(StringID::kOpenMap, to_prefix_string(Icon::kOpen));
  inject_icon(StringID::kOpenTileset, to_prefix_string(Icon::kOpen));
  inject_icon(StringID::kSave, to_prefix_string(Icon::kSave));
  inject_icon(StringID::kSaveAs, to_prefix_string(Icon::kSaveAs));
  inject_icon(StringID::kClose, to_prefix_string(Icon::kCloseFile));
  inject_icon(StringID::kQuit, to_prefix_string(Icon::kQuit));

  // Edit
  inject_icon(StringID::kUndo, to_prefix_string(Icon::kUndo));
  inject_icon(StringID::kRedo, to_prefix_string(Icon::kRedo));
  inject_icon(StringID::kStampTool, to_prefix_string(Icon::kStamp));
  inject_icon(StringID::kEraserTool, to_prefix_string(Icon::kEraser));
  inject_icon(StringID::kBucketTool, to_prefix_string(Icon::kBucket));
  inject_icon(StringID::kObjectSelectorTool, to_prefix_string(Icon::kSelect));
  inject_icon(StringID::kRectangleTool, to_prefix_string(Icon::kRectangle));
  inject_icon(StringID::kEllipseTool, to_prefix_string(Icon::kEllipse));
  inject_icon(StringID::kPointTool, to_prefix_string(Icon::kPoint));
  inject_icon(StringID::kOpenComponentEditor, to_prefix_string(Icon::kComponent));
  inject_icon(StringID::kOpenSettings, to_prefix_string(Icon::kSettings));

  // View
  inject_icon(StringID::kWidgetsMenu, to_prefix_string(Icon::kWidget));
  inject_icon(StringID::kThemeMenu, to_prefix_string(Icon::kTheme));
  inject_icon(StringID::kPanUp, to_prefix_string(Icon::kMoveUp));
  inject_icon(StringID::kPanDown, to_prefix_string(Icon::kMoveDown));
  inject_icon(StringID::kPanLeft, to_prefix_string(Icon::kMoveLeft));
  inject_icon(StringID::kPanRight, to_prefix_string(Icon::kMoveRight));
  inject_icon(StringID::kCenterViewport, to_prefix_string(Icon::kAlignCenter));
  inject_icon(StringID::kIncreaseZoom, to_prefix_string(Icon::kZoomIn));
  inject_icon(StringID::kDecreaseZoom, to_prefix_string(Icon::kZoomOut));
  inject_icon(StringID::kResetZoom, to_prefix_string(Icon::kZoomReset));
  inject_icon(StringID::kToggleGrid, to_prefix_string(Icon::kGrid));
  inject_icon(StringID::kToggleUi, to_prefix_string(Icon::kEye));

  // Map
  inject_icon(StringID::kAddTileset, to_prefix_string(Icon::kImage));
  inject_icon(StringID::kResize, to_prefix_string(Icon::kResize));
  inject_icon(StringID::kFixInvalidTiles, to_prefix_string(Icon::kRepair));
  inject_icon(StringID::kExportAsMenu, to_prefix_string(Icon::kExport));

  // Help
  inject_icon(StringID::kAboutTactile, to_prefix_string(Icon::kQuestion));
  inject_icon(StringID::kReportBug, to_prefix_string(Icon::kBug));
  inject_icon(StringID::kOpenCredits, to_prefix_string(Icon::kCode));

  // Debug
  inject_icon(StringID::kOpenDebugger, to_prefix_string(Icon::kDebug));
  inject_icon(StringID::kOpenStyleEditor, to_prefix_string(Icon::kBrush));
  inject_icon(StringID::kOpenDemoWindow, to_prefix_string(Icon::kToolbox));
  inject_icon(StringID::kOpenStorageDir, to_prefix_string(Icon::kOpen));

  inject_icon(StringID::kCreateProperty, to_prefix_string(Icon::kCreate));
  inject_icon(StringID::kRemoveProperty, to_prefix_string(Icon::kRemove));
  inject_icon(StringID::kRenameProperty, to_prefix_string(Icon::kEdit));

  inject_icon(StringID::kCreateTileset, to_prefix_string(Icon::kCreate));
  inject_icon(StringID::kCreateLayer, to_prefix_string(Icon::kCreate));

  inject_icon(StringID::kShowMetadata, to_prefix_string(Icon::kInfo));
  inject_icon(StringID::kSelectImage, to_prefix_string(Icon::kImage));
  inject_icon(StringID::kTileLayerItem, to_prefix_string(Icon::kTileLayer));
  inject_icon(StringID::kObjectLayerItem, to_prefix_string(Icon::kObjectLayer));
  inject_icon(StringID::kGroupLayerItem, to_prefix_string(Icon::kGroupLayer));
}

auto Language::get_id() const -> LanguageID
{
  return mID;
}

}  // namespace tactile::ui
