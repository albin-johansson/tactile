// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/i18n/language_parser.hpp"

#include <utility>  // move, to_underlying

#include <magic_enum.hpp>

#include "tactile/base/container/vector.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/debug/performance.hpp"
#include "tactile/core/io/ini.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile::ui {
inline namespace language_parser {

[[nodiscard]]
auto _get_misc_names() -> HashMap<StringView, StringID>
{
  return {
    {"ok", StringID::kOK},
  };
}

[[nodiscard]]
auto _get_verb_names() -> HashMap<StringView, StringID>
{
  return {
    {"cancel", StringID::kCancel},
    {"apply", StringID::kApply},
    {"close", StringID::kClose},
    {"create", StringID::kCreate},
    {"undo", StringID::kUndo},
    {"redo", StringID::kRedo},
    {"save", StringID::kSave},
    {"quit", StringID::kQuit},
  };
}

[[nodiscard]]
auto _get_noun_names() -> HashMap<StringView, StringID>
{
  return {
    {"int", StringID::kInt},
    {"int2", StringID::kInt2},
    {"int3", StringID::kInt3},
    {"int4", StringID::kInt4},
    {"float", StringID::kFloat},
    {"float2", StringID::kFloat2},
    {"float3", StringID::kFloat3},
    {"float4", StringID::kFloat4},
    {"bool", StringID::kBool},
    {"string", StringID::kString},
    {"color", StringID::kColor},
    {"object", StringID::kObject},
    {"path", StringID::kPath},
    {"row", StringID::kRow},
    {"rows", StringID::kRows},
    {"column", StringID::kColumn},
    {"columns", StringID::kColumns},
    {"orientation", StringID::kOrientation},
    {"tile_width", StringID::kTileWidth},
    {"tile_height", StringID::kTileHeight},
    {"light_themes", StringID::kLightThemes},
    {"dark_themes", StringID::kDarkThemes},
  };
}

[[nodiscard]]
auto _get_adjective_names() -> HashMap<StringView, StringID>
{
  return {
    {"orthogonal", StringID::kOrthogonal},
    {"hexagonal", StringID::kHexagonal},
  };
}

[[nodiscard]]
auto _get_action_names() -> HashMap<StringView, StringID>
{
  return {
    {"create_map", StringID::kCreateMap},
    {"open", StringID::kOpen},
    {"open_map", StringID::kOpenMap},
    {"open_tileset", StringID::kOpenTileset},
    {"open_component_editor", StringID::kOpenComponentEditor},
    {"open_settings", StringID::kOpenSettings},
    {"reset_layout", StringID::kResetLayout},
    {"save_as", StringID::kSaveAs},
    {"reopen_last_closed_file", StringID::kReopenLastClosedFile},
    {"clear_file_history", StringID::kClearFileHistory},
    {"increase_font_size", StringID::kIncreaseFontSize},
    {"decrease_font_size", StringID::kDecreaseFontSize},
    {"reset_font_size", StringID::kResetFontSize},
    {"increase_zoom", StringID::kIncreaseZoom},
    {"decrease_zoom", StringID::kDecreaseZoom},
    {"reset_zoom", StringID::kResetZoom},
    {"center_viewport", StringID::kCenterViewport},
    {"toggle_grid", StringID::kToggleGrid},
    {"pan_up", StringID::kPanUp},
    {"pan_down", StringID::kPanDown},
    {"pan_left", StringID::kPanLeft},
    {"pan_right", StringID::kPanRight},
    {"highlight_active_layer", StringID::kHighlightActiveLayer},
    {"toggle_ui", StringID::kToggleUi},
    {"stamp_tool", StringID::kStampTool},
    {"eraser_tool", StringID::kEraserTool},
    {"bucket_tool", StringID::kBucketTool},
    {"object_selector_tool", StringID::kObjectSelectorTool},
    {"rectangle_tool", StringID::kRectangleTool},
    {"ellipse_tool", StringID::kEllipseTool},
    {"point_tool", StringID::kPointTool},
    {"add_tileset", StringID::kAddTileset},
    {"add_row", StringID::kAddRow},
    {"add_column", StringID::kAddColumn},
    {"remove_row", StringID::kRemoveRow},
    {"remove_column", StringID::kRemoveColumn},
    {"resize", StringID::kResize},
    {"fix_invalid_tiles", StringID::kFixInvalidTiles},
    {"show_metadata", StringID::kShowMetadata},
    {"report_bug", StringID::kReportBug},
    {"about_tactile", StringID::kAboutTactile},
    {"open_credits", StringID::kOpenCredits},
    {"open_debugger", StringID::kOpenDebugger},
    {"open_style_editor", StringID::kOpenStyleEditor},
    {"open_demo_window", StringID::kOpenDemoWindow},
    {"open_storage_dir", StringID::kOpenStorageDir},
  };
}

[[nodiscard]]
auto _get_menu_names() -> HashMap<StringView, StringID>
{
  return {
    {"file", StringID::kFileMenu},
    {"edit", StringID::kEditMenu},
    {"view", StringID::kViewMenu},
    {"map", StringID::kMapMenu},
    {"tileset", StringID::kTilesetMenu},
    {"help", StringID::kHelpMenu},
    {"debug", StringID::kDebugMenu},
    {"recent_files", StringID::kRecentFilesMenu},
    {"widgets", StringID::kWidgetsMenu},
    {"theme", StringID::kThemeMenu},
    {"export_as", StringID::kExportAsMenu},
  };
}

[[nodiscard]]
auto _get_widget_names() -> HashMap<StringView, StringID>
{
  return {
    {"layer_dock", StringID::kLayerDock},
    {"tileset_dock", StringID::kTilesetDock},
    {"property_dock", StringID::kPropertyDock},
    {"component_dock", StringID::kComponentDock},
    {"animation_dock", StringID::kAnimationDock},
    {"log_dock", StringID::kLogDock},
    {"style_editor", StringID::kStyleEditorWidget},
  };
}

void _parse_section(const StringMap<IniSection>& ini,
                    const StringView section_name,
                    const HashMap<StringView, StringID>& name_mapping,
                    Vector<String>& strings)
{
  if (const auto* section = find_in(ini, section_name)) {
    for (const auto& [key, value] : *section) {
      if (const auto* string_id = find_in(name_mapping, key)) {
        strings[std::to_underlying(*string_id)] = value;
      }
    }
  }
}

[[nodiscard]]
auto _validate_strings(Vector<String>& strings,
                       const Language* fallback) -> Result<void>
{
  usize index {0};

  for (auto& string : strings) {
    if (string.empty()) {
      const StringID string_id {index};

      if (fallback != nullptr) {
        string = fallback->get(string_id);
      }
      else {
        TACTILE_LOG_ERROR("String with ID {} ('{}') is not translated",
                          index,
                          magic_enum::enum_name(string_id));
        return unexpected(make_error(GenericError::kInvalidFile));
      }
    }

    ++index;
  }

  return kOK;
}

}  // namespace language_parser

LanguageParser::LanguageParser()
  : mMiscNames {_get_misc_names()},
    mVerbNames {_get_verb_names()},
    mNounNames {_get_noun_names()},
    mAdjectiveNames {_get_adjective_names()},
    mActionNames {_get_action_names()},
    mMenuNames {_get_menu_names()},
    mWidgetNames {_get_widget_names()}
{}

auto LanguageParser::parse(const LanguageID id,
                           const Path& path,
                           const Language* fallback) const -> Result<Language>
{
  const SetLogScope log_scope {"LanguageParser"};
  const ScopeProfiler profiler {"parse"};

  return parse_ini(path)
      .transform([this](const IniData& ini) {
        Vector<String> strings {};
        strings.resize(std::to_underlying(StringID::kMAX));

        _parse_section(ini, "misc", mMiscNames, strings);
        _parse_section(ini, "verb", mVerbNames, strings);
        _parse_section(ini, "noun", mNounNames, strings);
        _parse_section(ini, "adjective", mAdjectiveNames, strings);
        _parse_section(ini, "action", mActionNames, strings);
        _parse_section(ini, "menu", mMenuNames, strings);
        _parse_section(ini, "widget", mWidgetNames, strings);

        return strings;
      })
      .and_then([id, fallback](Vector<String>&& strings) {
        return _validate_strings(strings, fallback).transform([id, &strings] {
          return Language {id, std::move(strings)};
        });
      });
}

auto parse_language_from_disk(const LanguageID id) -> Result<Language>
{
  LanguageParser parser {};

  Path path {};
  switch (id) {
    case LanguageID::kAmericanEnglish: path = "assets/lang/en.ini"; break;
    case LanguageID::kBritishEnglish:  path = "assets/lang/en_GB.ini"; break;
    case LanguageID::kSwedish:         path = "assets/lang/sv.ini"; break;
    default:                           return unexpected(make_error(GenericError::kInvalidParam));
  }

  return parser.parse(id, path);
}

}  // namespace tactile::ui
