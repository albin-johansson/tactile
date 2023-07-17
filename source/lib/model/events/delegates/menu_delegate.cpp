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

#include <imgui.h>

#include "model/documents/document_system.hpp"
#include "model/events/command_events.hpp"
#include "model/events/component_events.hpp"
#include "model/events/file_events.hpp"
#include "model/events/font_events.hpp"
#include "model/events/misc_events.hpp"
#include "model/events/setting_events.hpp"
#include "model/events/tileset_events.hpp"
#include "model/events/tool_events.hpp"
#include "model/events/view_events.hpp"
#include "model/events/viewport_events.hpp"
#include "ui/conversions.hpp"
#include "ui/dialog/about_dialog.hpp"
#include "ui/dialog/godot_export_dialog.hpp"

namespace tactile {

void on_menu_action(Registry& registry,
                    Dispatcher& dispatcher,
                    const MenuActionEvent& event)
{
  const auto document_entity = sys::get_active_document(registry);

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
      dispatcher.enqueue<ShowSaveAsDialogEvent>();
      break;

    case MenuAction::Close:
      dispatcher.enqueue<CloseDocumentEvent>(document_entity);
      break;

    case MenuAction::Quit:
      dispatcher.enqueue<QuitEvent>();
      break;

    case MenuAction::ReopenLastFile:
      dispatcher.enqueue<ReopenLastClosedFileEvent>();
      break;

    case MenuAction::ClearFileHistory:
      dispatcher.enqueue<ClearFileHistoryEvent>();
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
      dispatcher.enqueue<ShowComponentEditorEvent>();
      break;

    case MenuAction::OpenSettings:
      dispatcher.enqueue<ShowSettingsEvent>();
      break;

    case MenuAction::CenterViewport:
      dispatcher.enqueue<CenterViewportEvent>(document_entity);
      break;

    case MenuAction::ToggleGrid:
      dispatcher.enqueue<ToggleGridEvent>();
      break;

    case MenuAction::IncreaseZoom:
      dispatcher.enqueue<IncreaseViewportZoomEvent>(document_entity,
                                                    as_float2(ImGui::GetIO().MousePos));
      break;

    case MenuAction::DecreaseZoom:
      dispatcher.enqueue<DecreaseViewportZoomEvent>(document_entity,
                                                    as_float2(ImGui::GetIO().MousePos));
      break;

    case MenuAction::ResetZoom:
      dispatcher.enqueue<ResetViewportZoomEvent>(document_entity);
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
      dispatcher.enqueue<PanViewportUpEvent>(document_entity);
      break;

    case MenuAction::PanDown:
      dispatcher.enqueue<PanViewportDownEvent>(document_entity);
      break;

    case MenuAction::PanRight:
      dispatcher.enqueue<PanViewportRightEvent>(document_entity);
      break;

    case MenuAction::PanLeft:
      dispatcher.enqueue<PanViewportLeftEvent>(document_entity);
      break;

    case MenuAction::HighlightLayer:
      dispatcher.enqueue<ToggleHighlightLayerEvent>();
      break;

    case MenuAction::ToggleUi:
      dispatcher.enqueue<ToggleUiEvent>();
      break;

    case MenuAction::InspectMap:
      dispatcher.enqueue<InspectMapEvent>();
      break;

    case MenuAction::CreateTileset:
      dispatcher.enqueue<ShowNewTilesetDialogEvent>();
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
      dispatcher.enqueue<ShowGodotExportDialogEvent>();
      break;

    case MenuAction::InspectTileset:
      dispatcher.enqueue<InspectTilesetEvent>();
      break;

    case MenuAction::OpenDemoWindow:
      // TODO event
      break;

    case MenuAction::OpenStyleEditor:
      // TODO event
      break;

    case MenuAction::ShowAbout:
      dispatcher.enqueue<ShowAboutDialogEvent>();
      break;

    case MenuAction::ShowAboutImGui:
      dispatcher.enqueue<ShowAboutImGuiDialogEvent>();
      break;

    case MenuAction::ShowCredits:
      dispatcher.enqueue<ShowCreditsDialogEvent>();
      break;

    case MenuAction::ReportIssue:
      dispatcher.enqueue<OpenUrlEvent>(
          "https://github.com/albin-johansson/tactile/issues/new");
      break;
  }
}

}  // namespace tactile
