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

#include "app.hpp"

#include <utility>  // move

#include <boost/stacktrace.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include "cfg/configuration.hpp"
#include "core/commands/commands.hpp"
#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/events/map_events.hpp"
#include "core/events/misc_events.hpp"
#include "core/events/tileset_events.hpp"
#include "core/events/viewport_events.hpp"
#include "core/model.hpp"
#include "core/tilesets/tileset_info.hpp"
#include "core/tools/tool_manager.hpp"
#include "core/utils/texture_manager.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "editor/shortcuts/shortcuts.hpp"
#include "editor/ui/dialogs/save_as_dialog.hpp"
#include "editor/ui/fonts.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/layers/layer_dock.hpp"
#include "editor/ui/menus/edit_menu.hpp"
#include "editor/ui/menus/file_menu.hpp"
#include "editor/ui/menus/map_menu.hpp"
#include "editor/ui/properties/property_dock.hpp"
#include "editor/ui/shared/dialog_state.hpp"
#include "editor/ui/shared/dialogs.hpp"
#include "editor/ui/tilesets/tileset_dock.hpp"
#include "editor/ui/ui.hpp"
#include "editor/ui/viewport/map_viewport.hpp"
#include "editor/ui/viewport/viewport_widget.hpp"
#include "io/maps/parser/parse_map.hpp"
#include "io/maps/restore_map_from_ir.hpp"
#include "io/maps/save_document.hpp"
#include "io/persistence/history.hpp"
#include "io/persistence/preferences.hpp"
#include "io/persistence/session.hpp"
#include "misc/assert.hpp"

namespace tactile {

/// Tracks visibility of widgets for the "Toggle UI" feature.
struct WidgetShowState final
{
  bool prev_show_layer_dock : 1 {};
  bool prev_show_tileset_dock : 1 {};
  bool prev_show_property_dock : 1 {};
  bool prev_show_log_dock : 1 {};
  bool prev_show_component_dock : 1 {};
};

struct App::Data final
{
  AppConfiguration* config{}; /* Non-owning */
  entt::dispatcher  dispatcher;
  DocumentModel     model;
  TextureManager    textures;
  WidgetShowState   widget_show_state;
  bool              reload_fonts : 1 {};
};

App::App(AppConfiguration* configuration)
    : AEventLoop{configuration}
    , mData{std::make_unique<Data>()}
{
  mData->config = configuration;

  subscribe_to_events();
  load_default_shortcuts();

  ui::init_dialogs();
  ui::load_icons(mData->textures);
}

App::~App() noexcept = default;

void App::on_startup()
{
  io::load_file_history();

  if (io::get_preferences().will_restore_last_session()) {
    io::restore_last_session(mData->model, mData->textures);
  }

  auto& window = mData->config->window();
  window.show();
}

void App::on_shutdown()
{
  save_current_files_to_history();
  io::save_preferences();
  io::save_session(mData->model);
  io::save_file_history();

  auto& window = mData->config->window();
  window.hide();
}

void App::on_pre_update()
{
  if (mData->reload_fonts) {
    ui::reload_fonts();
    mData->reload_fonts = false;
  }
}

void App::on_update()
{
  auto& data = *mData;
  data.dispatcher.update();
  data.model.update();
  ui::update_widgets(data.model, data.dispatcher);
}

void App::on_event(const cen::event_handler& handler)
{
  switch (handler.type().value()) {
    case cen::event_type::key_up:
      [[fallthrough]];
    case cen::event_type::key_down:
      on_keyboard_event(handler.get<cen::keyboard_event>());
      break;

    case cen::event_type::mouse_wheel:
      on_mouse_wheel_event(handler.get<cen::mouse_wheel_event>());
      break;

    default:
      break;
  }
}

auto App::active_document() -> ADocument*
{
  return mData->model.active_document();
}

auto App::active_map_document() -> MapDocument*
{
  return mData->model.active_map();
}

auto App::active_tileset_document() -> TilesetDocument*
{
  return mData->model.active_tileset();
}

void App::subscribe_to_events()
{
  // clang-format off
  auto& d = get_dispatcher();

  d.sink<UndoEvent>().connect<&App::on_undo>(this);
  d.sink<RedoEvent>().connect<&App::on_redo>(this);
  d.sink<SetCommandCapacityEvent>().connect<&App::on_set_command_capacity>(this);

  d.sink<SaveEvent>().connect<&App::on_save>(this);
  d.sink<SaveAsEvent>().connect<&App::on_save_as>(this);
  d.sink<OpenSaveAsDialogEvent>().connect<&App::on_open_save_as_dialog>(this);
  d.sink<ShowSettingsEvent>().connect<&ui::show_settings_dialog>();

  d.sink<InspectMapEvent>().connect<&App::on_inspect_map>(this);
  d.sink<InspectTilesetEvent>().connect<&App::on_inspect_tileset>(this);

  d.sink<ShowNewMapDialogEvent>().connect<&ui::show_map_creation_dialog>();
  d.sink<ShowOpenMapDialogEvent>().connect<&ui::show_map_selector_dialog>();
  d.sink<CreateMapEvent>().connect<&App::on_create_map>(this);
  d.sink<OpenMapEvent>().connect<&App::on_open_map>(this);

  d.sink<OpenDocumentEvent>().connect<&App::on_open_document>(this);
  d.sink<CloseDocumentEvent>().connect<&App::on_close_document>(this);
  d.sink<SelectDocumentEvent>().connect<&App::on_select_document>(this);

  d.sink<SelectToolEvent>().connect<&App::on_select_tool>(this);
  d.sink<ToolPressedEvent>().connect<&App::on_tool_pressed>(this);
  d.sink<ToolDraggedEvent>().connect<&App::on_tool_dragged>(this);
  d.sink<ToolReleasedEvent>().connect<&App::on_tool_released>(this);
  d.sink<ToolEnteredEvent>().connect<&App::on_tool_entered>(this);
  d.sink<ToolExitedEvent>().connect<&App::on_tool_exited>(this);

  d.sink<StampSequenceEvent>().connect<&App::on_stamp_sequence>(this);
  d.sink<SetStampRandomizerEvent>().connect<&App::on_set_stamp_randomizer_event>(this);
  d.sink<EraserSequenceEvent>().connect<&App::on_eraser_sequence>(this);
  d.sink<FloodEvent>().connect<&App::on_flood>(this);
  d.sink<AddRectangleEvent>().connect<&App::on_add_rectangle>(this);
  d.sink<AddEllipseEvent>().connect<&App::on_add_ellipse>(this);
  d.sink<AddPointEvent>().connect<&App::on_add_point>(this);

  d.sink<CenterViewportEvent>().connect<&ui::center_map_viewport>();
  d.sink<UpdateTilesetViewportLimitsEvent>().connect<&App::on_update_tileset_viewport_limits>(this);

  d.sink<OffsetTilesetViewportEvent>().connect<&App::on_offset_tileset_viewport>(this);
  d.sink<OffsetDocumentViewportEvent>().connect<&App::on_offset_document_viewport>(this);

  d.sink<PanLeftEvent>().connect<&App::on_pan_left>(this);
  d.sink<PanRightEvent>().connect<&App::on_pan_right>(this);
  d.sink<PanUpEvent>().connect<&App::on_pan_up>(this);
  d.sink<PanDownEvent>().connect<&App::on_pan_down>(this);

  d.sink<IncreaseZoomEvent>().connect<&App::on_increase_zoom>(this);
  d.sink<DecreaseZoomEvent>().connect<&App::on_decrease_zoom>(this);
  d.sink<ResetZoomEvent>().connect<&App::on_reset_zoom>(this);

  d.sink<ResetFontSizeEvent>().connect<&App::on_reset_font_size>(this);
  d.sink<IncreaseFontSizeEvent>().connect<&App::on_increase_font_size>(this);
  d.sink<DecreaseFontSizeEvent>().connect<&App::on_decrease_font_size>(this);

  d.sink<ShowTilesetCreationDialogEvent>().connect<&ui::show_tileset_creation_dialog>();
  d.sink<LoadTilesetEvent>().connect<&App::on_load_tileset>(this);
  d.sink<RemoveTilesetEvent>().connect<&App::on_remove_tileset>(this);
  d.sink<SelectTilesetEvent>().connect<&App::on_select_tileset>(this);
  d.sink<SetTilesetSelectionEvent>().connect<&App::on_set_tileset_selection>(this);
  d.sink<RenameTilesetEvent>().connect<&App::on_rename_tileset>(this);

  d.sink<AddRowEvent>().connect<&App::on_add_row>(this);
  d.sink<AddColumnEvent>().connect<&App::on_add_column>(this);
  d.sink<RemoveRowEvent>().connect<&App::on_remove_row>(this);
  d.sink<RemoveColumnEvent>().connect<&App::on_remove_column>(this);
  d.sink<ResizeMapEvent>().connect<&App::on_resize_map>(this);
  d.sink<FixTilesInMapEvent>().connect<&App::on_fix_tiles_in_map>(this);
  d.sink<OpenResizeMapDialogEvent>().connect<&App::on_open_resize_map_dialog>(this);

  d.sink<AddLayerEvent>().connect<&App::on_add_layer>(this);
  d.sink<RemoveLayerEvent>().connect<&App::on_remove_layer>(this);
  d.sink<SelectLayerEvent>().connect<&App::on_select_layer>(this);
  d.sink<MoveLayerUpEvent>().connect<&App::on_move_layer_up>(this);
  d.sink<MoveLayerDownEvent>().connect<&App::on_move_layer_down>(this);
  d.sink<DuplicateLayerEvent>().connect<&App::on_duplicate_layer>(this);
  d.sink<SetLayerOpacityEvent>().connect<&App::on_set_layer_opacity>(this);
  d.sink<SetLayerVisibleEvent>().connect<&App::on_set_layer_visible>(this);
  d.sink<OpenRenameLayerDialogEvent>().connect<&App::on_open_rename_layer_dialog>(this);
  d.sink<RenameLayerEvent>().connect<&App::on_rename_layer>(this);

  d.sink<SetObjectNameEvent>().connect<&App::on_set_object_name>(this);
  d.sink<MoveObjectEvent>().connect<&App::on_move_object>(this);
  d.sink<SetObjectVisibleEvent>().connect<&App::on_set_object_visible>(this);
  d.sink<SetObjectTagEvent>().connect<&App::on_set_object_tag>(this);
  d.sink<SpawnObjectContextMenuEvent>().connect<&App::on_spawn_object_context_menu>(this);

  d.sink<ShowAddPropertyDialogEvent>().connect<&App::on_show_add_property_dialog>(this);
  d.sink<ShowRenamePropertyDialogEvent>().connect<&App::on_show_rename_property_dialog>(this);
  d.sink<ShowChangePropertyTypeDialogEvent>().connect<&App::on_show_change_property_type_dialog>(this);
  d.sink<AddPropertyEvent>().connect<&App::on_add_property>(this);
  d.sink<RemovePropertyEvent>().connect<&App::on_remove_property>(this);
  d.sink<RenamePropertyEvent>().connect<&App::on_rename_property>(this);
  d.sink<UpdatePropertyEvent>().connect<&App::on_update_property>(this);
  d.sink<ChangePropertyTypeEvent>().connect<&App::on_change_property_type>(this);
  d.sink<InspectContextEvent>().connect<&App::on_inspect_context>(this);

  d.sink<OpenComponentEditorEvent>().connect<&App::on_open_component_editor>(this);
  d.sink<DefineComponentEvent>().connect<&App::on_define_component>(this);
  d.sink<UndefComponentEvent>().connect<&App::on_undef_component>(this);
  d.sink<RenameComponentEvent>().connect<&App::on_rename_component>(this);
  d.sink<AddComponentAttrEvent>().connect<&App::on_add_component_attr>(this);
  d.sink<RemoveComponentAttrEvent>().connect<&App::on_remove_component_attr>(this);
  d.sink<RenameComponentAttrEvent>().connect<&App::on_rename_component_attr>(this);
  d.sink<DuplicateComponentAttrEvent>().connect<&App::on_duplicate_component_attr>(this);
  d.sink<SetComponentAttrTypeEvent>().connect<&App::on_set_component_attr_type>(this);
  d.sink<UpdateComponentEvent>().connect<&App::on_update_component>(this);

  d.sink<AttachComponentEvent>().connect<&App::on_attach_component>(this);
  d.sink<DetachComponentEvent>().connect<&App::on_detach_component>(this);
  d.sink<UpdateAttachedComponentEvent>().connect<&App::on_update_attached_component>(this);
  d.sink<ResetAttachedComponentEvent>().connect<&App::on_reset_attached_component>(this);

  d.sink<ToggleUiEvent>().connect<&App::on_toggle_ui>(this);
  d.sink<ReloadFontsEvent>().connect<&App::on_reload_fonts>(this);
  d.sink<QuitEvent>().connect<&App::on_quit>(this);
  // clang-format on
}

auto App::get_dispatcher() -> entt::dispatcher&
{
  return mData->dispatcher;
}

void App::save_current_files_to_history()
{
  mData->model.each([this](const UUID& id) {
    const auto document = mData->model.get_document(id);
    if (document->is_map() && document->has_path()) {
      io::add_file_to_history(document->get_path());
    }
  });
}

void App::on_keyboard_event(cen::keyboard_event event)
{
  /* We don't care about these modifiers, they are just noise */
  event.set_modifier(cen::key_mod::caps, false);
  event.set_modifier(cen::key_mod::num, false);
  event.set_modifier(cen::key_mod::mode, false);

  auto& data = *mData;
  update_shortcuts(data.model, event, data.dispatcher);
}

void App::on_mouse_wheel_event(const cen::mouse_wheel_event& event)
{
  /* There doesn't seem to be a good way to handle mouse "wheel" events using the public
     ImGui APIs. Otherwise, it would be nicer to keep this code closer to the actual
     widgets. */

  const auto* document = active_document();
  if (document && !ImGui::GetTopMostPopupModal()) {
    if (ui::is_mouse_within_viewport()) {
      ui::viewport_widget_mouse_wheel_event_handler(document->get_viewport(),
                                                    mData->dispatcher,
                                                    event);
    }
    else if (document->is_map() && ui::is_tileset_dock_hovered()) {
      const auto& mapDocument = mData->model.require_active_map();

      const auto& map = mapDocument.get_map();
      const auto& tilesets = map.get_tilesets();

      if (const auto tilesetId = tilesets.active_tileset_id()) {
        const auto& tilesetRef = tilesets.get_ref(*tilesetId);
        ui::tileset_dock_mouse_wheel_event_handler(tilesetRef, event, mData->dispatcher);
      }
    }
  }
}

void App::on_undo()
{
  if (auto* document = active_document()) {
    auto& commands = document->get_history();
    commands.undo();
  }
}

void App::on_redo()
{
  if (auto* document = active_document()) {
    auto& commands = document->get_history();
    commands.redo();
  }
}

void App::on_set_command_capacity(const SetCommandCapacityEvent& event)
{
  mData->model.set_command_capacity(event.capacity);
}

void App::on_save()
{
  // TODO ability to save tileset documents
  if (auto* document = active_map_document()) {
    if (document->has_path()) {
      io::save_document(*document);

      document->get_history().mark_as_clean();
      document->set_name(document->get_path().filename().string());
    }
    else {
      on_open_save_as_dialog();
    }
  }
}

void App::on_save_as(const SaveAsEvent& event)
{
  if (auto* document = active_document()) {
    document->set_path(event.path);
    on_save();
  }
}

void App::on_open_save_as_dialog()
{
  if (active_document() != nullptr) {
    ui::show_save_as_dialog(mData->dispatcher);
  }
}

void App::on_inspect_map()
{
  if (auto* document = active_map_document()) {
    document->select_context(document->get_map().get_uuid());
  }
}

void App::on_inspect_tileset()
{
  if (auto* document = active_tileset_document()) {
    document->select_context(document->view_tileset().get_uuid());
  }
}

void App::on_create_map(const CreateMapEvent& event)
{
  const auto id = mData->model.add_map({event.tile_width, event.tile_height},
                                       event.row_count,
                                       event.column_count);
  mData->model.select_document(id);
}

void App::on_close_document(const CloseDocumentEvent& event)
{
  const auto document = mData->model.get_document(event.id);

  if (document->is_map() && document->has_path()) {
    io::set_last_closed_file(document->get_path());
  }

  mData->model.close_document(event.id);
}

void App::on_open_document(const OpenDocumentEvent& event)
{
  mData->model.open_document(event.document_id);
}

// TODO consider renaming event (when standalone tileset documents can be parsed)
void App::on_open_map(const OpenMapEvent& event)
{
  if (mData->model.has_document_with_path(event.path)) {
    const auto id = mData->model.get_id_for_path(event.path);

    if (mData->model.is_open(id)) {
      spdlog::warn("Tried to open map that was already open!");
    }
    else {
      mData->model.open_document(id);
    }

    return;
  }

  const auto ir = io::parse_map(event.path);
  if (ir.error() == io::ParseError::None) {
    restore_map_from_ir(ir, mData->model, mData->textures);
    io::add_file_to_history(event.path);
  }
  else {
    ui::get_dialogs().map_parse_error.show(ir.error());
  }
}

void App::on_select_document(const SelectDocumentEvent& event)
{
  mData->model.select_document(event.id);
}

void App::on_select_tool(const SelectToolEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& tools = document->get_tools();
    tools.select_tool(event.type, mData->model, mData->dispatcher);
  }
}

void App::on_tool_pressed(const ToolPressedEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& tools = document->get_tools();
    tools.on_pressed(mData->model, mData->dispatcher, event.info);
  }
}

void App::on_tool_dragged(const ToolDraggedEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& tools = document->get_tools();
    tools.on_dragged(mData->model, mData->dispatcher, event.info);
  }
}

void App::on_tool_released(const ToolReleasedEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& tools = document->get_tools();
    tools.on_released(mData->model, mData->dispatcher, event.info);
  }
}

void App::on_tool_entered()
{
  if (auto* document = active_map_document()) {
    auto& tools = document->get_tools();
    tools.on_entered(mData->model, mData->dispatcher);
  }
}

void App::on_tool_exited()
{
  if (auto* document = active_map_document()) {
    auto& tools = document->get_tools();
    tools.on_exited(mData->model, mData->dispatcher);
  }
}

void App::on_stamp_sequence(StampSequenceEvent event)
{
  if (auto* document = active_map_document()) {
    document->register_stamp_sequence(event.layer_id,
                                      std::move(event.old_state),
                                      std::move(event.sequence));
  }
}

void App::on_set_stamp_randomizer_event(const SetStampRandomizerEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& tools = document->get_tools();
    tools.set_stamp_random_mode(event.enabled);
  }
}

void App::on_eraser_sequence(EraserSequenceEvent event)
{
  if (auto* document = active_map_document()) {
    document->register_eraser_sequence(event.layer_id, std::move(event.old_state));
  }
}

void App::on_flood(const FloodEvent& event)
{
  if (auto* document = active_map_document()) {
    document->flood(event.layer_id, event.origin, event.replacement);
  }
}

void App::on_add_rectangle(const AddRectangleEvent& event)
{
  if (auto* document = active_map_document()) {
    document->add_rectangle(event.layer_id, event.pos, event.size);
  }
}

void App::on_add_ellipse(const AddEllipseEvent& event)
{
  if (auto* document = active_map_document()) {
    document->add_ellipse(event.layer_id, event.pos, event.size);
  }
}

void App::on_add_point(const AddPointEvent& event)
{
  if (auto* document = active_map_document()) {
    document->add_point(event.layer_id, event.pos);
  }
}

void App::on_update_tileset_viewport_limits(const UpdateTilesetViewportLimitsEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& tilesetRef = document->get_map().get_tilesets().get_ref(event.tileset_id);
    auto& viewport = tilesetRef.get_viewport();
    viewport.set_limits({event.min_offset, event.max_offset});
  }
}

void App::on_offset_document_viewport(const OffsetDocumentViewportEvent& event)
{
  // FIXME possible crash if tileset is removed when offsetting viewport
  if (auto* document = active_document()) {
    auto& viewport = document->get_viewport();
    viewport.offset(event.delta);
  }
}

void App::on_offset_tileset_viewport(const OffsetTilesetViewportEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& tilesetRef = document->get_map().get_tilesets().get_ref(event.tileset_id);
    auto& viewport = tilesetRef.get_viewport();
    viewport.offset(event.delta);
  }
}

void App::on_pan_left()
{
  if (auto* document = active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_left();
  }
}

void App::on_pan_right()
{
  if (auto* document = active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_right();
  }
}

void App::on_pan_up()
{
  if (auto* document = active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_up();
  }
}

void App::on_pan_down()
{
  if (auto* document = active_document()) {
    auto& viewport = document->get_viewport();
    viewport.pan_down();
  }
}

void App::on_increase_zoom()
{
  if (auto* document = active_document()) {
    auto&      viewport = document->get_viewport();
    const auto mousePos = ImGui::GetIO().MousePos;
    viewport.zoom_in(Vector2f{mousePos.x, mousePos.y});
  }
}

void App::on_decrease_zoom()
{
  if (auto* document = active_document()) {
    auto&      viewport = document->get_viewport();
    const auto mousePos = ImGui::GetIO().MousePos;
    viewport.zoom_out(Vector2f{mousePos.x, mousePos.y});
  }
}

void App::on_reset_zoom()
{
  if (auto* document = active_document()) {
    auto& viewport = document->get_viewport();
    viewport.set_tile_size({64, 64});
  }
}

void App::on_reset_font_size()
{
  io::get_preferences().set_font_size(ui::get_default_font_size());
  mData->reload_fonts = true;
}

void App::on_increase_font_size()
{
  auto& prefs = io::get_preferences();

  TACTILE_ASSERT(prefs.font_size() + 2 <= ui::get_max_font_size());
  prefs.set_font_size(prefs.font_size() + 2);

  mData->reload_fonts = true;
}

void App::on_decrease_font_size()
{
  auto& prefs = io::get_preferences();

  TACTILE_ASSERT(prefs.font_size() - 2 >= ui::get_min_font_size());
  prefs.set_font_size(prefs.font_size() - 2);

  mData->reload_fonts = true;
}

void App::on_load_tileset(const LoadTilesetEvent& event)
{
  if (auto info = mData->textures.load(event.path)) {
    mData->model.add_tileset({.texture_path = info->path,
                              .texture_id = info->id,
                              .texture_size = info->size,
                              .tile_size = event.tile_size});
  }
  else {
    spdlog::error("Failed to load tileset texture!");
  }
}

void App::on_remove_tileset(const RemoveTilesetEvent& event)
{
  mData->model.remove_tileset(event.tileset_id);
}

void App::on_select_tileset(const SelectTilesetEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& tilesets = document->get_map().get_tilesets();
    tilesets.select_tileset(event.tileset_id);
  }
}

void App::on_set_tileset_selection(const SetTilesetSelectionEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& tilesets = document->get_map().get_tilesets();

    const auto tilesetId = tilesets.active_tileset_id().value();
    auto&      tilesetRef = tilesets.get_ref(tilesetId);

    tilesetRef.set_selection(event.selection);
  }
}

void App::on_rename_tileset(const RenameTilesetEvent& event)
{
  auto document = mData->model.get_tileset(event.tileset_id);
  document->rename_tileset(event.name);
}

void App::on_add_row()
{
  if (auto* map = active_map_document()) {
    map->add_row();
  }
}

void App::on_add_column()
{
  if (auto* map = active_map_document()) {
    map->add_column();
  }
}

void App::on_remove_row()
{
  if (auto* map = active_map_document()) {
    map->remove_row();
  }
}

void App::on_remove_column()
{
  if (auto* map = active_map_document()) {
    map->remove_column();
  }
}

void App::on_resize_map(const ResizeMapEvent& event)
{
  if (auto* map = active_map_document()) {
    map->resize(event.row_count, event.col_count);
  }
}

void App::on_fix_tiles_in_map()
{
  if (auto* map = active_map_document()) {
    map->fix_tiles();
  }
}

void App::on_open_resize_map_dialog()
{
  if (auto* document = active_map_document()) {
    const auto& map = document->get_map();
    ui::get_dialogs().resize_map.show(map.row_count(), map.column_count());
  }
}

void App::on_add_layer(const AddLayerEvent& event)
{
  if (auto* map = active_map_document()) {
    map->add_layer(event.type);
  }
}

void App::on_remove_layer(const RemoveLayerEvent& event)
{
  if (auto* map = active_map_document()) {
    map->remove_layer(event.layer_id);
  }
}

void App::on_select_layer(const SelectLayerEvent& event)
{
  if (auto* document = active_map_document()) {
    auto& map = document->get_map();
    map.select_layer(event.layer_id);
  }
}

void App::on_move_layer_up(const MoveLayerUpEvent& event)
{
  if (auto* map = active_map_document()) {
    map->move_layer_up(event.id);
  }
}

void App::on_move_layer_down(const MoveLayerDownEvent& event)
{
  if (auto* map = active_map_document()) {
    map->move_layer_down(event.layer_id);
  }
}

void App::on_duplicate_layer(const DuplicateLayerEvent& event)
{
  if (auto* map = active_map_document()) {
    map->duplicate_layer(event.layer_id);
  }
}

void App::on_set_layer_opacity(const SetLayerOpacityEvent& event)
{
  if (auto* map = active_map_document()) {
    map->set_layer_opacity(event.layer_id, event.opacity);
  }
}

void App::on_set_layer_visible(const SetLayerVisibleEvent& event)
{
  if (auto* map = active_map_document()) {
    map->set_layer_visible(event.layer_id, event.visible);
  }
}

void App::on_open_rename_layer_dialog(const OpenRenameLayerDialogEvent& event)
{
  ui::show_rename_layer_dialog(event.layer_id);
}

void App::on_rename_layer(const RenameLayerEvent& event)
{
  if (auto* map = active_map_document()) {
    map->rename_layer(event.layer_id, event.name);
  }
}

void App::on_set_object_name(const SetObjectNameEvent& event)
{
  if (auto* map = active_map_document()) {
    map->set_object_name(event.object_id, event.name);
  }
}

void App::on_move_object(const MoveObjectEvent& event)
{
  if (auto* map = active_map_document()) {
    map->move_object(event.object_id, event.previous, event.updated);
  }
}

void App::on_set_object_visible(const SetObjectVisibleEvent& event)
{
  if (auto* document = active_map_document()) {
    document->set_object_visible(event.object_id, event.visible);
  }
}

void App::on_set_object_tag(const SetObjectTagEvent& event)
{
  if (auto* document = active_map_document()) {
    document->set_object_tag(event.object_id, event.tag);
  }
}

void App::on_spawn_object_context_menu(const SpawnObjectContextMenuEvent&)
{
  ui::open_object_context_menu();
}

void App::on_show_add_property_dialog()
{
  if (auto* document = active_document()) {
    const auto& contextId = document->active_context_id();
    ui::get_dialogs().add_property.open(contextId);
  }
}

void App::on_show_rename_property_dialog(const ShowRenamePropertyDialogEvent& event)
{
  if (const auto* document = active_document()) {
    const auto& contextId = document->active_context_id();
    ui::get_dialogs().rename_property.open(contextId, event.current_name);
  }
}

void App::on_show_change_property_type_dialog(
    const ShowChangePropertyTypeDialogEvent& event)
{
  if (const auto* document = active_document()) {
    const auto& contextId = document->active_context_id();
    ui::get_dialogs().change_property_type.show(contextId,
                                                event.name,
                                                event.current_type);
  }
}

void App::on_add_property(const AddPropertyEvent& event)
{
  if (auto* document = active_document()) {
    document->add_property(event.context_id, event.name, event.type);
  }
}

void App::on_remove_property(const RemovePropertyEvent& event)
{
  if (auto* document = active_document()) {
    document->remove_property(event.context_id, event.name);
  }
}

void App::on_rename_property(const RenamePropertyEvent& event)
{
  if (auto* document = active_document()) {
    document->rename_property(event.context_id, event.old_name, event.new_name);
  }
}

void App::on_update_property(const UpdatePropertyEvent& event)
{
  if (auto* document = active_document()) {
    document->update_property(event.context_id, event.name, event.value);
  }
}

void App::on_change_property_type(const ChangePropertyTypeEvent& event)
{
  if (auto* document = active_document()) {
    document->change_property_type(event.context_id, event.name, event.type);
  }
}

void App::on_inspect_context(const InspectContextEvent& event)
{
  if (auto* document = active_document()) {
    document->select_context(event.context_id);
  }
}

void App::on_open_component_editor()
{
  ui::show_component_editor(mData->model);
}

void App::on_define_component(const DefineComponentEvent& event)
{
  if (auto* document = active_document()) {
    document->define_component(event.name);
  }
}

void App::on_undef_component(const UndefComponentEvent& event)
{
  if (auto* document = active_document()) {
    document->undef_component(event.component_id);
  }
}

void App::on_rename_component(const RenameComponentEvent& event)
{
  if (auto* document = active_document()) {
    document->rename_component(event.component_id, event.name);
  }
}

void App::on_add_component_attr(const AddComponentAttrEvent& event)
{
  if (auto* document = active_document()) {
    document->add_component_attribute(event.component_id, event.attr_name);
  }
}

void App::on_remove_component_attr(const RemoveComponentAttrEvent& event)
{
  if (auto* document = active_document()) {
    document->remove_component_attribute(event.component_id, event.attr_name);
  }
}

void App::on_rename_component_attr(const RenameComponentAttrEvent& event)
{
  if (auto* document = active_document()) {
    document->rename_component_attribute(event.component_id,
                                         event.current_name,
                                         event.updated_name);
  }
}

void App::on_duplicate_component_attr(const DuplicateComponentAttrEvent& event)
{
  if (auto* document = active_document()) {
    document->duplicate_component_attribute(event.component_id, event.attr_name);
  }
}

void App::on_set_component_attr_type(const SetComponentAttrTypeEvent& event)
{
  if (auto* document = active_document()) {
    document->set_component_attribute_type(event.component_id,
                                           event.attr_name,
                                           event.type);
  }
}

void App::on_update_component(const UpdateComponentEvent& event)
{
  if (auto* document = active_document()) {
    document->update_component(event.component_id, event.attr_name, event.value);
  }
}

void App::on_attach_component(const AttachComponentEvent& event)
{
  if (auto* document = active_document()) {
    document->attach_component(event.context_id, event.component_id);
  }
}

void App::on_detach_component(const DetachComponentEvent& event)
{
  if (auto* document = active_document()) {
    document->detach_component(event.context_id, event.component_id);
  }
}

void App::on_update_attached_component(const UpdateAttachedComponentEvent& event)
{
  if (auto* document = active_document()) {
    document->update_attached_component(event.context_id,
                                        event.component_id,
                                        event.attr_name,
                                        event.value);
  }
}

void App::on_reset_attached_component(const ResetAttachedComponentEvent& event)
{
  if (auto* document = active_document()) {
    document->reset_attached_component(event.context_id, event.component_id);
  }
}

void App::on_toggle_ui()
{
  if (ImGui::GetTopMostPopupModal() != nullptr) {
    return;
  }

  static bool show = false;

  auto& prefs = io::get_preferences();
  auto& state = mData->widget_show_state;

  if (!show) {
    state.prev_show_layer_dock = prefs.is_layer_dock_visible();
    state.prev_show_tileset_dock = prefs.is_tileset_dock_visible();
    state.prev_show_property_dock = prefs.is_property_dock_visible();
    state.prev_show_log_dock = prefs.is_log_dock_visible();
    state.prev_show_component_dock = prefs.is_component_dock_visible();
  }

  prefs.set_layer_dock_visible(show);
  prefs.set_tileset_dock_visible(show);
  prefs.set_property_dock_visible(show);
  prefs.set_component_dock_visible(show);
  prefs.set_log_dock_visible(show);

  if (show) {
    prefs.set_layer_dock_visible(state.prev_show_layer_dock);
    prefs.set_tileset_dock_visible(state.prev_show_tileset_dock);
    prefs.set_property_dock_visible(state.prev_show_property_dock);
    prefs.set_log_dock_visible(state.prev_show_log_dock);
    prefs.set_component_dock_visible(state.prev_show_component_dock);
  }

  show = !show;
}

void App::on_reload_fonts()
{
  mData->reload_fonts = true;
}

void App::on_quit()
{
  stop();
}

}  // namespace tactile
