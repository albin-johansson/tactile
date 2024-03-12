// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <utility>  // move

#include <centurion/system.hpp>
#include <entt/signal/dispatcher.hpp>

#include "common/util/filesystem.hpp"
#include "handlers/event_handlers.hpp"
#include "model/event/all.hpp"
#include "model/file_history.hpp"
#include "model/model.hpp"
#include "model/settings.hpp"
#include "runtime/app_context.hpp"
#include "ui/dialog/about_dialog.hpp"
#include "ui/dialog/credits_dialog.hpp"
#include "ui/dialog/godot_export_dialog.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/comp/component_editor.hpp"
#include "ui/menu/menu.hpp"
#include "ui/ui.hpp"

namespace tactile {
namespace {

void dispatch_menu_action(const MenuAction action)
{
  const auto& model = get_model();
  auto& dispatcher = get_dispatcher();
  auto& settings = get_settings();

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
      dispatcher.enqueue<CloseDocumentEvent>(model.get_active_document_id().value());
      break;

    case MenuAction::Quit:
      dispatcher.enqueue<QuitEvent>();
      break;

    case MenuAction::ReopenLastClosedFile: {
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
      ui::open_component_editor_dialog(model);
      break;

    case MenuAction::OpenSettings:
      ui::open_settings_dialog();
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
      ui::open_godot_export_dialog();
      break;

    case MenuAction::InspectTileset:
      dispatcher.enqueue<InspectTilesetEvent>();
      break;

    case MenuAction::DemoWindow:
      break;

    case MenuAction::StyleEditor:
      break;

    case MenuAction::AboutTactile:
      ui::open_about_dialog();
      break;

    case MenuAction::AboutDearImGui:
      ui::open_about_dear_imgui_dialog();
      break;

    case MenuAction::ReportIssue:
      cen::open_url("https://github.com/albin-johansson/tactile/issues/new");
      break;

    case MenuAction::Credits:
      ui::open_credits_dialog();
      break;
  }
}

void on_menu_item_event(const MenuItemEvent event)
{
  dispatch_menu_action(event.action);
}

}  // namespace

void install_menu_event_handler()
{
  auto& dispatcher = get_dispatcher();
  dispatcher.sink<MenuItemEvent>().connect<&on_menu_item_event>();
}

}  // namespace tactile
