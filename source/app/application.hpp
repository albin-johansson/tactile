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

#pragma once

#include <centurion/event.hpp>
#include <entt/fwd.hpp>

#include "core/common/memory.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/component_events.hpp"
#include "editor/events/document_events.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/events/object_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/fwd.hpp"
#include "loop.hpp"

namespace tactile {

class AppConfiguration;

/**
 * \brief Represents the heart of the Tactile application.
 *
 * \details This class handles the main poll/update/render application loop, along with
 * the basic initialization and configuration of all aspects of the Tactile application.
 */
class Application final : AEventLoop
{
 public:
  explicit Application(AppConfiguration* configuration);

  ~Application() noexcept override;

  using AEventLoop::start;

 protected:
  void on_startup() override;

  void on_shutdown() override;

  void on_pre_update() override;

  void on_update() override;

  void on_event(const cen::event_handler& handler) override;

 private:
  struct Data;
  Unique<Data> mData;

  [[nodiscard]] auto active_document() -> ADocument*;

  void subscribe_to_events();

  [[nodiscard]] auto get_dispatcher() -> entt::dispatcher&;

  void save_current_files_to_history();

  void on_keyboard_event(cen::keyboard_event event);

  void on_mouse_wheel_event(const cen::mouse_wheel_event& event);

  void on_undo();

  void on_redo();

  void on_set_command_capacity(const SetCommandCapacityEvent& event);

  void on_save();

  void on_save_as(const SaveAsEvent& event);

  void on_open_save_as_dialog();

  void on_show_map_properties();

  void on_create_map(const CreateMapEvent& event);

  void on_close_document(const CloseDocumentEvent& event);

  void on_open_document(const OpenDocumentEvent& event);

  void on_open_map(const OpenMapEvent& event);

  void on_select_document(const SelectDocumentEvent& event);

  void on_select_tool(const SelectToolEvent& event);

  void on_tool_pressed(const ToolPressedEvent& event);

  void on_tool_dragged(const ToolDraggedEvent& event);

  void on_tool_released(const ToolReleasedEvent& event);

  void on_tool_entered();

  void on_tool_exited();

  void on_stamp_sequence(StampSequenceEvent event);

  void on_set_stamp_randomizer_event(const SetStampRandomizerEvent& event);

  void on_eraser_sequence(EraserSequenceEvent event);

  void on_flood(const FloodEvent& event);

  void on_add_rectangle(const AddRectangleEvent& event);

  void on_add_ellipse(const AddEllipseEvent& event);

  void on_add_point(const AddPointEvent& event);

  void on_offset_viewport(const OffsetViewportEvent& event);

  void on_offset_bound_viewport(const OffsetBoundViewportEvent& event);

  void on_pan_left();

  void on_pan_right();

  void on_pan_up();

  void on_pan_down();

  void on_increase_zoom();

  void on_decrease_zoom();

  void on_reset_zoom();

  void on_reset_font_size();

  void on_increase_font_size();

  void on_decrease_font_size();

  void on_add_tileset(const AddTilesetEvent& event);

  void on_remove_tileset(const RemoveTilesetEvent& event);

  void on_select_tileset(const SelectTilesetEvent& event);

  void on_set_tileset_selection(const SetTilesetSelectionEvent& event);

  void on_set_tileset_name(const SetTilesetNameEvent& event);

  void on_add_row();

  void on_add_column();

  void on_remove_row();

  void on_remove_column();

  void on_resize_map(const ResizeMapEvent& event);

  void on_open_resize_map_dialog();

  void on_add_layer(const AddLayerEvent& event);

  void on_remove_layer(const RemoveLayerEvent& event);

  void on_select_layer(const SelectLayerEvent& event);

  void on_move_layer_up(const MoveLayerUpEvent& event);

  void on_move_layer_down(const MoveLayerDownEvent& event);

  void on_duplicate_layer(const DuplicateLayerEvent& event);

  void on_set_layer_opacity(const SetLayerOpacityEvent& event);

  void on_set_layer_visible(const SetLayerVisibleEvent& event);

  void on_open_rename_layer_dialog(const OpenRenameLayerDialogEvent& event);

  void on_rename_layer(const RenameLayerEvent& event);

  void on_set_object_name(const SetObjectNameEvent& event);

  void on_move_object(const MoveObjectEvent& event);

  void on_set_object_visibility(const SetObjectVisibilityEvent& event);

  void on_set_object_tag(const SetObjectTagEvent& event);

  void on_spawn_object_context_menu(const SpawnObjectContextMenuEvent& event);

  void on_show_rename_property_dialog(const ShowRenamePropertyDialogEvent& event);

  void on_show_change_property_type_dialog(
      const ShowChangePropertyTypeDialogEvent& event);

  void on_add_property(const AddPropertyEvent& event);

  void on_remove_property(const RemovePropertyEvent& event);

  void on_rename_property(const RenamePropertyEvent& event);

  void on_update_property(const UpdatePropertyEvent& event);

  void on_change_property_type(const ChangePropertyTypeEvent& event);

  void on_inspect_context(const InspectContextEvent& event);

  void on_open_component_editor();

  void on_create_component_def(const CreateComponentDefEvent& event);

  void on_remove_component_def(const RemoveComponentDefEvent& event);

  void on_rename_component_def(const RenameComponentDefEvent& event);

  void on_create_component_attr(const CreateComponentAttrEvent& event);

  void on_remove_component_attr(const RemoveComponentAttrEvent& event);

  void on_rename_component_attr(const RenameComponentAttrEvent& event);

  void on_duplicate_component_attr(const DuplicateComponentAttrEvent& event);

  void on_set_component_attr_type(const SetComponentAttrTypeEvent& event);

  void on_update_component_def_attr(const UpdateComponentDefAttrEvent& event);

  void on_add_component(const AddComponentEvent& event);

  void on_remove_component(const RemoveComponentEvent& event);

  void on_update_component(const UpdateComponentEvent& event);

  void on_reset_component_values(const ResetComponentValuesEvent& event);

  void on_toggle_ui();

  void on_reload_fonts();

  void on_quit();
};

}  // namespace tactile
