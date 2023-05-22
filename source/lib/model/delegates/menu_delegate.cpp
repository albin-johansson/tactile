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

#include "menu_delegate.hpp"

#include <centurion/system.hpp>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "model/event/all.hpp"
#include "model/file_history.hpp"
#include "model/settings.hpp"
#include "model/systems/document_system.hpp"
#include "ui/dialog/about_dialog.hpp"
#include "ui/dialog/credits_dialog.hpp"
#include "ui/dialog/godot_export_dialog.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/comp/component_editor.hpp"
#include "ui/ui.hpp"

namespace tactile {

void on_menu_action(Model& model, Dispatcher& dispatcher, const MenuActionEvent& event)
{
  spdlog::trace("[MenuActionEvent] action: {}", magic_enum::enum_name(event.action));

  auto& settings = model.get<Settings>();

  switch (event.action) {
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

    case MenuAction::Close:
      dispatcher.enqueue<CloseDocumentEvent>(sys::get_active_document(model));
      break;

    case MenuAction::Quit:
      dispatcher.enqueue<QuitEvent>();
      break;

    case MenuAction::ReopenLastFile: {
      // TODO this will need to be tweaked if tileset documents viewing will be supported
      Path file_path {get_file_history().last_closed_file.value()};
      dispatcher.enqueue<OpenMapEvent>(std::move(file_path));
      break;
    }
    case MenuAction::ClearFileHistory:
      clear_file_history();
      break;

    case MenuAction::Undo:
      dispatcher.enqueue<UndoEvent>();
      break;

    case MenuAction::Redo:
      dispatcher.enqueue<RedoEvent>();
      break;

    case MenuAction::EnableStamp:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
      break;

    case MenuAction::EnableEraser:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
      break;

    case MenuAction::EnableBucket:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Bucket);
      break;

    case MenuAction::EnableObjectSelector:
      dispatcher.enqueue<SelectToolEvent>(ToolType::ObjectSelection);
      break;

    case MenuAction::EnableRectangle:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Rectangle);
      break;

    case MenuAction::EnableEllipse:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Ellipse);
      break;

    case MenuAction::EnablePoint:
      dispatcher.enqueue<SelectToolEvent>(ToolType::Point);
      break;

    case MenuAction::OpenComponentEditor:
      ui::open_component_editor_dialog(model);
      break;

    case MenuAction::OpenSettings:
      ui::open_settings_dialog(settings);
      break;

    case MenuAction::CenterViewport:
      dispatcher.enqueue<CenterViewportEvent>();
      break;

    case MenuAction::ToggleGrid:
      dispatcher.enqueue<ToggleGridEvent>();
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
      settings.negate_flag(SETTINGS_HIGHLIGHT_ACTIVE_LAYER_BIT);
      break;

    case MenuAction::ToggleUi:
      dispatcher.enqueue<ToggleUiEvent>();
      break;

    case MenuAction::InspectMap:
      dispatcher.enqueue<InspectMapEvent>();
      break;

    case MenuAction::CreateTileset:
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
      dispatcher.enqueue<ShowResizeMapDialogEvent>();
      break;

    case MenuAction::ExportGodotScene:
      ui::open_godot_export_dialog();
      break;

    case MenuAction::InspectTileset:
      dispatcher.enqueue<InspectTilesetEvent>();
      break;

    case MenuAction::OpenDemoWindow:
      break;

    case MenuAction::OpenStyleEditor:
      break;

    case MenuAction::ShowAbout:
      ui::open_about_dialog();
      break;

    case MenuAction::ShowAboutImGui:
      ui::open_about_dear_imgui_dialog();
      break;

    case MenuAction::ShowCredits:
      ui::open_credits_dialog();
      break;

    case MenuAction::ReportIssue:
      cen::open_url("https://github.com/albin-johansson/tactile/issues/new");
      break;
  }
}

}  // namespace tactile