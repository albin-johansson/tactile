/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "menu_event_handler.hpp"

#include <centurion/system.hpp>
#include <entt/signal/dispatcher.hpp>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "core/util/filesystem.hpp"
#include "editor/app_context.hpp"
#include "editor/ui/dialog/dialog_state.hpp"
#include "editor/ui/dialog/dialogs.hpp"
#include "editor/ui/ui.hpp"
#include "io/proto/history.hpp"
#include "model/event/all.hpp"
#include "model/model.hpp"

namespace tactile {

void dispatch_menu_action(MenuAction action)
{
  const auto& model = get_model();
  auto& dispatcher = get_dispatcher();
  auto& prefs = io::get_preferences();

  spdlog::debug("dispatch_menu_action {}", magic_enum::enum_name(action));
  switch (action) {
    case MenuAction::NewMap:
      dispatcher.enqueue<ShowNewMapDialogEvent>();
      break;

    case MenuAction::OpenMap:
      dispatcher.enqueue<ShowOpenMapDialogEvent>();
      break;

    case MenuAction::Save:
      dispatcher.enqueue<SaveEvent>();
      break;

    case MenuAction::SaveAs:
      dispatcher.enqueue<OpenSaveAsDialogEvent>();
      break;

    case MenuAction::CloseDocument:
      dispatcher.enqueue<CloseDocumentEvent>(model.active_document_id().value());
      break;

    case MenuAction::Quit:
      dispatcher.enqueue<QuitEvent>();
      break;

    case MenuAction::ReopenLastClosedFile:
      // TODO this will need to be tweaked if tileset documents viewing will be supported
      dispatcher.enqueue<OpenMapEvent>(to_path(io::last_closed_file()));
      break;

    case MenuAction::ClearFileHistory:
      io::clear_file_history();
      break;

    case MenuAction::Undo:
      dispatcher.enqueue<UndoEvent>();
      break;

    case MenuAction::Redo:
      dispatcher.enqueue<RedoEvent>();
      break;

    case MenuAction::StampTool:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
      break;

    case MenuAction::EraserTool:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
      break;

    case MenuAction::BucketTool:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
      break;

    case MenuAction::ObjectSelectionTool:
      dispatcher.enqueue<SelectToolEvent>(ToolType::ObjectSelection);
      break;

    case MenuAction::RectangleTool:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Rectangle);
      break;

    case MenuAction::EllipseTool:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Ellipse);
      break;

    case MenuAction::PointTool:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Point);
      break;

    case MenuAction::ComponentEditor:
      ui::get_dialogs().component_editor.show(model);
      break;

    case MenuAction::OpenSettings:
      ui::get_dialogs().settings.show();
      break;

    case MenuAction::CenterViewport:
      dispatcher.enqueue<CenterViewportEvent>();
      break;

    case MenuAction::ToggleGrid:
      prefs.show_grid = !prefs.show_grid;
      break;

    case MenuAction::IncreaseZoom:
      dispatcher.enqueue<IncreaseZoomEvent>();
      break;

    case MenuAction::DecreaseZoom:
      dispatcher.enqueue<DecreaseZoomEvent>();
      break;

    case MenuAction::ResetZoom:
      dispatcher.enqueue<ResetZoomEvent>();
      break;

    case MenuAction::IncreaseFontSize:
      dispatcher.enqueue<IncreaseFontSizeEvent>();
      break;

    case MenuAction::DecreaseFontSize:
      dispatcher.enqueue<DecreaseFontSizeEvent>();
      break;

    case MenuAction::ResetFontSize:
      dispatcher.enqueue<ResetFontSizeEvent>();
      break;

    case MenuAction::PanUp:
      dispatcher.enqueue<PanUpEvent>();
      break;

    case MenuAction::PanDown:
      dispatcher.enqueue<PanDownEvent>();
      break;

    case MenuAction::PanRight:
      dispatcher.enqueue<PanRightEvent>();
      break;

    case MenuAction::PanLeft:
      dispatcher.enqueue<PanLeftEvent>();
      break;

    case MenuAction::HighlightLayer:
      prefs.highlight_active_layer = !prefs.highlight_active_layer;
      break;

    case MenuAction::ToggleUi:
      dispatcher.enqueue<ToggleUiEvent>();
      break;

    case MenuAction::InspectMap:
      dispatcher.enqueue<InspectMapEvent>();
      break;

    case MenuAction::AddTileset:
      dispatcher.enqueue<ShowTilesetCreationDialogEvent>();
      break;

    case MenuAction::AddRow:
      dispatcher.enqueue<AddRowEvent>();
      break;

    case MenuAction::AddColumn:
      dispatcher.enqueue<AddColumnEvent>();
      break;

    case MenuAction::RemoveRow:
      dispatcher.enqueue<RemoveRowEvent>();
      break;

    case MenuAction::RemoveColumn:
      dispatcher.enqueue<RemoveColumnEvent>();
      break;

    case MenuAction::FixInvalidTiles:
      dispatcher.enqueue<FixTilesInMapEvent>();
      break;

    case MenuAction::ResizeMap:
      dispatcher.enqueue<OpenResizeMapDialogEvent>();
      break;

    case MenuAction::ExportGodotScene:
      ui::get_dialogs().godot_export.open();
      break;

    case MenuAction::InspectTileset:
      dispatcher.enqueue<InspectTilesetEvent>();
      break;

    case MenuAction::DemoWindow:
      break;

    case MenuAction::StyleEditor:
      break;

    case MenuAction::AboutTactile:
      ui::get_dialogs().about.show();
      break;

    case MenuAction::AboutDearImGui:
      ui::show_about_dear_imgui_dialog();
      break;

    case MenuAction::ReportIssue:
      cen::open_url("https://github.com/albin-johansson/tactile/issues/new");
      break;

    case MenuAction::Credits:
      ui::get_dialogs().credits.show();
      break;
  }
}

}  // namespace tactile
