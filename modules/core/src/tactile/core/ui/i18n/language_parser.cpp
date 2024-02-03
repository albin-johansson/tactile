// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/i18n/language_parser.hpp"

#include <utility>  // to_underlying

#include "tactile/core/ui/i18n/string_id.hpp"
#include "tactile/foundation/container/hash_map.hpp"
#include "tactile/foundation/container/lookup.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/ini.hpp"

namespace tactile::core {
namespace {

inline const HashMap<StringView, StringID> kGenericNames = {
  {"ok", StringID::kOK},
  {"cancel", StringID::kCancel},
  {"apply", StringID::kApply},
  {"close", StringID::kClose},
  {"create", StringID::kCreate},
};

inline const HashMap<StringView, StringID> kWidgetNames = {
  {"layer_dock", StringID::kLayerDock},
  {"tileset_dock", StringID::kTilesetDock},
  {"property_dock", StringID::kPropertyDock},
  {"component_dock", StringID::kComponentDock},
  {"animation_dock", StringID::kAnimationDock},
  {"log_dock", StringID::kLogDock},
  {"style_editor", StringID::kStyleEditorWidget},
};

inline const HashMap<StringView, StringID> kMenuNames = {
  {"file", StringID::kFileMenu},
  {"edit", StringID::kEditMenu},
  {"view", StringID::kViewMenu},
  {"map", StringID::kMapMenu},
  {"tileset", StringID::kTilesetMenu},
  {"help", StringID::kHelpMenu},
  {"debug", StringID::kDebugMenu},
  {"recent_files", StringID::kRecentFilesMenu},
  {"widgets", StringID::kWidgetsMenu},
  {"export_as", StringID::kExportAsMenu},
};

inline const HashMap<StringView, StringID> kActionNames = {
  {"undo", StringID::kUndo},
  {"redo", StringID::kRedo},
  {"create_map", StringID::kCreateMap},
  {"open_map", StringID::kOpenMap},
  {"open_tileset", StringID::kOpenTileset},
  {"save", StringID::kSave},
  {"save_as", StringID::kSaveAs},
  {"center_viewport", StringID::kCenterViewport},
  {"increase_zoom", StringID::kIncreaseZoom},
  {"decrease_zoom", StringID::kDecreaseZoom},
  {"reset_zoom", StringID::kResetZoom},
  {"toggle_grid", StringID::kToggleGrid},
  {"increase_font_size", StringID::kIncreaseFontSize},
  {"decrease_font_size", StringID::kDecreaseFontSize},
  {"reset_font_size", StringID::kResetFontSize},
  {"pan_up", StringID::kPanUp},
  {"pan_down", StringID::kPanDown},
  {"pan_left", StringID::kPanLeft},
  {"pan_right", StringID::kPanRight},
  {"report_bug", StringID::kReportBug},
  {"open_demo_window", StringID::kOpenDemoWindow},
  {"open_style_editor", StringID::kOpenStyleEditor},
};

inline const HashMap<StringView, StringID> kTypeNames = {
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
};

void _parse_section(const StringMap<IniSection>& ini,
                    const StringView section_name,
                    const HashMap<StringView, StringID>& name_mapping,
                    Vector<String>& strings)
{
  if (const auto* section = find_in(ini, section_name)) {
    for (const auto& [string_name, string] : *section) {
      if (const auto* string_id = find_in(name_mapping, string_name)) {
        strings[std::to_underlying(*string_id)] = string;
      }
    }
  }
}

}  // namespace

auto parse_language_strings(const FilePath& path) -> Result<Vector<String>>
{
  return parse_ini(path).transform([](const IniData& ini) {
    Vector<String> strings {};
    strings.resize(std::to_underlying(StringID::kMAX));

    _parse_section(ini, "generic", kGenericNames, strings);
    _parse_section(ini, "widget", kWidgetNames, strings);
    _parse_section(ini, "menu", kMenuNames, strings);
    _parse_section(ini, "action", kActionNames, strings);
    _parse_section(ini, "type", kTypeNames, strings);

    return strings;
  });
}

}  // namespace tactile::core
