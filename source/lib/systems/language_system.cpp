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

#include "language_system.hpp"

#include <utility>  // move

#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/util/lookup.hpp"
#include "core/language.hpp"
#include "io/lang/language_parser.hpp"
#include "model/settings.hpp"

namespace tactile::sys {

void load_languages(Model& model)
{
  spdlog::debug("[IO] Loading languages");

  auto& languages = model.get<Languages>();

  auto en = parse_language("assets/lang/en.json");

  languages.strings[Lang::SV] = parse_language("assets/lang/sv.json", en);
  languages.strings[Lang::EN_GB] = parse_language("assets/lang/en_GB.json", en);
  languages.strings[Lang::EN] = std::move(en);
}

auto get_current_language_strings(const Model& model) -> const Strings&
{
  const auto& settings = model.get<Settings>();
  const auto& languages = model.get<Languages>();
  return lookup_in(languages.strings, settings.get_language());
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
      throw TactileError {"Unknown language"};
  }
}

auto get_string(const Strings& strings, const MenuAction action) -> String
{
  switch (action) {
    case MenuAction::NewMap:
      return strings.action.create_map;

    case MenuAction::OpenMap:
      return strings.action.open_map;

    case MenuAction::Save:
      return strings.action.save;

    case MenuAction::SaveAs:
      return strings.action.save_as;

    case MenuAction::Close:
      return strings.action.close_document;

    case MenuAction::Quit:
      return strings.action.exit;

    case MenuAction::ReopenLastFile:
      return strings.action.reopen_last_closed_file;

    case MenuAction::ClearFileHistory:
      return strings.action.clear_file_history;

    case MenuAction::Undo:
      return strings.action.undo;

    case MenuAction::Redo:
      return strings.action.redo;

    case MenuAction::EnableStamp:
      return strings.action.stamp_tool;

    case MenuAction::EnableEraser:
      return strings.action.eraser_tool;

    case MenuAction::EnableBucket:
      return strings.action.bucket_tool;

    case MenuAction::EnableObjectSelector:
      return strings.action.object_selection_tool;

    case MenuAction::EnableRectangle:
      return strings.action.rectangle_tool;

    case MenuAction::EnableEllipse:
      return strings.action.ellipse_tool;

    case MenuAction::EnablePoint:
      return strings.action.point_tool;

    case MenuAction::OpenComponentEditor:
      return strings.action.component_editor;

    case MenuAction::OpenSettings:
      return strings.action.show_settings;

    case MenuAction::CenterViewport:
      return strings.action.center_viewport;

    case MenuAction::ToggleGrid:
      return strings.action.toggle_grid;

    case MenuAction::IncreaseZoom:
      return strings.action.increase_zoom;

    case MenuAction::DecreaseZoom:
      return strings.action.decrease_zoom;

    case MenuAction::ResetZoom:
      return strings.action.reset_zoom;

    case MenuAction::IncreaseFontSize:
      return strings.action.increase_font_size;

    case MenuAction::DecreaseFontSize:
      return strings.action.decrease_font_size;

    case MenuAction::ResetFontSize:
      return strings.action.reset_font_size;

    case MenuAction::PanUp:
      return strings.action.pan_up;

    case MenuAction::PanDown:
      return strings.action.pan_down;

    case MenuAction::PanRight:
      return strings.action.pan_right;

    case MenuAction::PanLeft:
      return strings.action.pan_left;

    case MenuAction::HighlightLayer:
      return strings.action.highlight_layer;

    case MenuAction::ToggleUi:
      return strings.action.toggle_ui;

    case MenuAction::InspectMap:
      return strings.action.inspect_map;

    case MenuAction::CreateTileset:
      return strings.action.create_tileset;

    case MenuAction::AddRow:
      return strings.action.add_row;

    case MenuAction::AddColumn:
      return strings.action.add_column;

    case MenuAction::RemoveRow:
      return strings.action.remove_row;

    case MenuAction::RemoveColumn:
      return strings.action.remove_column;

    case MenuAction::FixInvalidTiles:
      return strings.action.fix_invalid_tiles;

    case MenuAction::ResizeMap:
      return strings.action.resize_map;

    case MenuAction::ExportGodotScene:
      return strings.action.export_as_godot_scene;

    case MenuAction::InspectTileset:
      return strings.action.inspect_tileset;

    case MenuAction::OpenDemoWindow:
      return "Demo Window...";

    case MenuAction::OpenStyleEditor:
      return "Style Editor...";

    case MenuAction::ShowAbout:
      return strings.action.show_about;

    case MenuAction::ShowAboutImGui:
      return strings.action.about_dear_imgui;

    case MenuAction::ShowCredits:
      return strings.action.show_credits;

    case MenuAction::ReportIssue:
      return strings.action.report_issue;

    default:
      throw TactileError {"Missing menu action case"};
  }
}

}  // namespace tactile::sys
