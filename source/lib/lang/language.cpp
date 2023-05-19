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

#include "language.hpp"

#include <utility>  // move

#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/type/hash_map.hpp"
#include "common/util/assoc.hpp"
#include "io/lang/language_parser.hpp"
#include "lang/strings.hpp"
#include "model/context.hpp"

namespace tactile {
namespace {

inline HashMap<Lang, Strings> gLanguages;

}  // namespace

void load_languages()
{
  spdlog::debug("Loading languages...");

  auto en = parse_language("assets/lang/en.json");

  gLanguages[Lang::SV] = parse_language("assets/lang/sv.json", en);
  gLanguages[Lang::EN_GB] = parse_language("assets/lang/en_GB.json", en);

  gLanguages[Lang::EN] = std::move(en);
}

auto get_language(const Lang lang) -> const Strings&
{
  return lookup_in(gLanguages, lang);
}

auto get_current_language() -> const Strings&
{
  return lookup_in(gLanguages, get_global_settings().get_language());
}

auto get_language_name(const Lang lang) -> const char*
{
  switch (lang) {
    case Lang::EN:
      return "English (US)";

    case Lang::EN_GB:
      return "English (GB)";

    case Lang::SV:
      return "Svenska";

    default:
      throw TactileError {"Invalid language!"};
  }
}

auto get_label(const Strings& lang, const MenuAction action) -> String
{
  switch (action) {
    case MenuAction::NewMap:
      return lang.action.create_map;

    case MenuAction::OpenMap:
      return lang.action.open_map;

    case MenuAction::Save:
      return lang.action.save;

    case MenuAction::SaveAs:
      return lang.action.save_as;

    case MenuAction::Close:
      return lang.action.close_document;

    case MenuAction::Quit:
      return lang.action.exit;

    case MenuAction::ReopenLastFile:
      return lang.action.reopen_last_closed_file;

    case MenuAction::ClearFileHistory:
      return lang.action.clear_file_history;

    case MenuAction::Undo:
      return lang.action.undo;

    case MenuAction::Redo:
      return lang.action.redo;

    case MenuAction::EnableStamp:
      return lang.action.stamp_tool;

    case MenuAction::EnableEraser:
      return lang.action.eraser_tool;

    case MenuAction::EnableBucket:
      return lang.action.bucket_tool;

    case MenuAction::EnableObjectSelector:
      return lang.action.object_selection_tool;

    case MenuAction::EnableRectangle:
      return lang.action.rectangle_tool;

    case MenuAction::EnableEllipse:
      return lang.action.ellipse_tool;

    case MenuAction::EnablePoint:
      return lang.action.point_tool;

    case MenuAction::OpenComponentEditor:
      return lang.action.component_editor;

    case MenuAction::OpenSettings:
      return lang.action.show_settings;

    case MenuAction::CenterViewport:
      return lang.action.center_viewport;

    case MenuAction::ToggleGrid:
      return lang.action.toggle_grid;

    case MenuAction::IncreaseZoom:
      return lang.action.increase_zoom;

    case MenuAction::DecreaseZoom:
      return lang.action.decrease_zoom;

    case MenuAction::ResetZoom:
      return lang.action.reset_zoom;

    case MenuAction::IncreaseFontSize:
      return lang.action.increase_font_size;

    case MenuAction::DecreaseFontSize:
      return lang.action.decrease_font_size;

    case MenuAction::ResetFontSize:
      return lang.action.reset_font_size;

    case MenuAction::PanUp:
      return lang.action.pan_up;

    case MenuAction::PanDown:
      return lang.action.pan_down;

    case MenuAction::PanRight:
      return lang.action.pan_right;

    case MenuAction::PanLeft:
      return lang.action.pan_left;

    case MenuAction::HighlightLayer:
      return lang.action.highlight_layer;

    case MenuAction::ToggleUi:
      return lang.action.toggle_ui;

    case MenuAction::InspectMap:
      return lang.action.inspect_map;

    case MenuAction::CreateTileset:
      return lang.action.create_tileset;

    case MenuAction::AddRow:
      return lang.action.add_row;

    case MenuAction::AddColumn:
      return lang.action.add_column;

    case MenuAction::RemoveRow:
      return lang.action.remove_row;

    case MenuAction::RemoveColumn:
      return lang.action.remove_column;

    case MenuAction::FixInvalidTiles:
      return lang.action.fix_invalid_tiles;

    case MenuAction::ResizeMap:
      return lang.action.resize_map;

    case MenuAction::ExportGodotScene:
      return lang.action.export_as_godot_scene;

    case MenuAction::InspectTileset:
      return lang.action.inspect_tileset;

    case MenuAction::OpenDemoWindow:
      return "Demo Window...";

    case MenuAction::OpenStyleEditor:
      return "Style Editor...";

    case MenuAction::ShowAbout:
      return lang.action.show_about;

    case MenuAction::ShowAboutImGui:
      return lang.action.about_dear_imgui;

    case MenuAction::ShowCredits:
      return lang.action.show_credits;

    case MenuAction::ReportIssue:
      return lang.action.report_issue;

    default:
      throw TactileError {"Missing menu action case"};
  }
}

}  // namespace tactile
