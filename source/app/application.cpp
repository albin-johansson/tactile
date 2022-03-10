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

#include "application.hpp"

#include <utility>  // move, forward

#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <imgui_internal.h>

#include "application_events.hpp"
#include "cfg/configuration.hpp"
#include "core/components/attributes.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tools/tool_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "core/viewport.hpp"
#include "editor/commands/commands.hpp"
#include "editor/gui/dialogs/save_as_dialog.hpp"
#include "editor/gui/toolbar/toolbar.hpp"
#include "editor/gui/viewport/map_view.hpp"
#include "editor/gui/viewport/viewport_widget.hpp"
#include "editor/shortcuts/shortcuts.hpp"
#include "io/maps/parser/parse_map.hpp"
#include "io/maps/restore_document_from_ir.hpp"
#include "io/maps/save_document.hpp"
#include "io/persistence/history.hpp"
#include "io/persistence/preferences.hpp"
#include "io/persistence/session.hpp"
#include "misc/logging.hpp"

namespace tactile {
namespace {

template <typename Command, typename... Args>
void _execute(DocumentModel& model, Args&&... args)
{
  if (auto* document = model.active_document()) {
    auto& commands = document->commands;
    commands.push<Command>(document->registry, std::forward<Args>(args)...);
  }
  else {
    log_error("Could not execute a command due to no active document!");
  }
}

template <typename Command, typename... Args>
void _register(DocumentModel& model, Args&&... args)
{
  if (auto* document = model.active_document()) {
    auto& commands = document->commands;
    commands.push_without_redo<Command>(document->registry, std::forward<Args>(args)...);
  }
  else {
    log_error("Could not register a command due to no active document!");
  }
}

}  // namespace

Application::Application(AppConfiguration* configuration)
    : mConfiguration{configuration}
    , mIcons{mTextures}
{
  subscribe_to_events(*this);
  load_default_shortcuts();
}

auto Application::run() -> int
{
  auto& window = mConfiguration->window();

  load_file_history();

  if (get_preferences().will_restore_last_session()) {
    restore_last_session(mModel, mTextures);
  }

  window.show();

  const auto& io = ImGui::GetIO();
  while (!mQuit) {
    mKeyboard.refresh();
    poll_events();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    update_frame();
    ImGui::EndFrame();

    ImGui::Render();

    glViewport(0,
               0,
               static_cast<int>(io.DisplaySize.x),
               static_cast<int>(io.DisplaySize.y));
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    cen::gl::swap(window);
  }

  on_about_to_exit();
  window.hide();

  return 0;
}

void Application::on_about_to_exit()
{
  save_current_files_to_history();
  save_preferences();
  save_session(mModel);
  save_file_history();
}

void Application::save_current_files_to_history()
{
  for (const auto& [id, document] : mModel) {
    if (!document->path.empty()) {
      add_file_to_history(document->path);
    }
  }
}

void Application::poll_events()
{
  cen::event_handler handler;
  while (handler.poll()) {
    ImGui_ImplSDL2_ProcessEvent(handler.data());

    switch (handler.type().value()) {
      case cen::event_type::quit:
        mQuit = true;
        break;

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
}

void Application::on_keyboard_event(cen::keyboard_event event)
{
  /* We don't care about these modifiers, they are just noise */
  event.set_modifier(cen::key_mod::caps, false);
  event.set_modifier(cen::key_mod::num, false);
  event.set_modifier(cen::key_mod::mode, false);

  update_shortcuts(mModel, mWidgets, event, mDispatcher);
}

void Application::on_mouse_wheel_event(const cen::mouse_wheel_event& event)
{
  constexpr float scaling = 4.0f;

  const auto* registry = mModel.active_registry();
  if (registry && !ImGui::GetTopMostPopupModal()) {
    if (is_mouse_within_viewport()) {
      const auto& viewport = registry->ctx<Viewport>();
      const auto dx = static_cast<float>(event.x()) * (viewport.tile_width / scaling);
      const auto dy = static_cast<float>(event.y()) * (viewport.tile_height / scaling);
      mDispatcher.enqueue<OffsetViewportEvent>(-dx, dy);
    }
    else if (mWidgets.is_tileset_dock_hovered()) {
      const auto width = mWidgets.tileset_view_width();
      const auto height = mWidgets.tileset_view_height();
      if (width && height) {
        const auto entity = sys::find_active_tileset(*registry);
        TACTILE_ASSERT(entity != entt::null);

        const auto& viewport = registry->get<Viewport>(entity);

        const auto dx = static_cast<float>(event.x()) * (viewport.tile_width / scaling);
        const auto dy = static_cast<float>(event.y()) * (viewport.tile_height / scaling);
        mDispatcher.enqueue<OffsetBoundViewportEvent>(entity, -dx, dy, *width, *height);
      }
    }
  }
}

void Application::update_frame()
{
  mDispatcher.update();
  mModel.update();
  mWidgets.update(mModel, mIcons, mDispatcher);
}

void Application::on_undo()
{
  if (auto* document = mModel.active_document()) {
    document->commands.undo();
  }
}

void Application::on_redo()
{
  if (auto* document = mModel.active_document()) {
    document->commands.redo();
  }
}

void Application::on_set_command_capacity(const SetCommandCapacityEvent& event)
{
  mModel.set_command_capacity(event.capacity);
}

void Application::on_save()
{
  if (auto* document = mModel.active_document()) {
    if (!document->path.empty()) {
      save_document(*document);
      document->commands.mark_as_clean();

      auto& context = document->registry.ctx<comp::AttributeContext>();
      context.name = document->path.filename().string();
    }
    else {
      on_open_save_as_dialog();
    }
  }
}

void Application::on_save_as(const SaveAsEvent& event)
{
  if (auto* document = mModel.active_document()) {
    document->path = event.path;
    on_save();
  }
}

void Application::on_open_save_as_dialog()
{
  if (mModel.has_active_document()) {
    show_save_as_dialog(mDispatcher);
  }
}

void Application::on_show_settings()
{
  mWidgets.show_settings();
}

void Application::on_show_new_map_dialog()
{
  mWidgets.show_new_map_dialog();
}

void Application::on_show_open_map_dialog()
{
  mWidgets.show_open_map_dialog();
}

void Application::on_show_map_properties()
{
  if (auto* registry = mModel.active_registry()) {
    auto& current = registry->ctx<comp::ActiveAttributeContext>();
    current.entity = entt::null;
  }
}

void Application::on_create_map(const CreateMapEvent& event)
{
  const auto id = mModel.add_map(event.tile_width,
                                 event.tile_height,
                                 event.row_count,
                                 event.column_count);
  mModel.select_map(id);
}

void Application::on_close_map(const CloseMapEvent& event)
{
  if (mModel.has_path(event.id)) {
    set_last_closed_file(mModel.get_path(event.id));
  }
  mModel.remove_map(event.id);
}

void Application::on_open_map(const OpenMapEvent& event)
{
  /* Just silently ignore the request if the map is already open */
  if (mModel.has_document_with_path(event.path)) {
    log_warning("Tried to open map that was already open!");
    return;
  }

  const auto ir = parsing::parse_map(event.path);
  if (ir.error() == parsing::ParseError::none) {
    mModel.add_map(restore_document_from_ir(ir, mTextures));
    add_file_to_history(event.path);
  }
  else {
    mWidgets.show_map_import_error_dialog(ir.error());
  }
}

void Application::on_select_map(const SelectMapEvent& event)
{
  mModel.select_map(event.id);
}

void Application::on_select_tool(const SelectToolEvent& event)
{
  auto& registry = mModel.get_active_registry();
  sys::select_tool(registry, mDispatcher, event.type);
}

void Application::on_tool_pressed(const ToolPressedEvent& event)
{
  if (auto* registry = mModel.active_registry()) {
    sys::on_tool_pressed(*registry, mDispatcher, event.info);
  }
}

void Application::on_tool_dragged(const ToolDraggedEvent& event)
{
  if (auto* registry = mModel.active_registry()) {
    sys::on_tool_dragged(*registry, mDispatcher, event.info);
  }
}

void Application::on_tool_released(const ToolReleasedEvent& event)
{
  if (auto* registry = mModel.active_registry()) {
    sys::on_tool_released(*registry, mDispatcher, event.info);
  }
}

void Application::on_tool_entered()
{
  if (auto* registry = mModel.active_registry()) {
    sys::on_tool_entered(*registry, mDispatcher);
  }
}

void Application::on_tool_exited()
{
  if (auto* registry = mModel.active_registry()) {
    sys::on_tool_exited(*registry, mDispatcher);
  }
}

void Application::on_stamp_sequence(StampSequenceEvent event)
{
  _register<StampToolCmd>(mModel, std::move(event.old_state), std::move(event.sequence));
}

void Application::on_eraser_sequence(EraserSequenceEvent event)
{
  _register<EraserToolCmd>(mModel, std::move(event.old_state));
}

void Application::on_flood(const FloodEvent& event)
{
  _execute<BucketToolCmd>(mModel, event.origin, event.replacement);
}

void Application::on_add_rectangle(const AddRectangleEvent& event)
{
  _execute<RectangleToolCmd>(mModel, event.x, event.y, event.width, event.height);
}

void Application::on_add_ellipse(const AddEllipseEvent& event)
{
  _execute<EllipseToolCmd>(mModel, event.x, event.y, event.width, event.height);
}

void Application::on_add_point(const AddPointEvent& event)
{
  _execute<PointToolCmd>(mModel, event.x, event.y);
}

void Application::on_center_viewport()
{
  center_viewport();
}

void Application::on_offset_viewport(const OffsetViewportEvent& event)
{
  auto& registry = mModel.get_active_registry();
  sys::OffsetViewport(registry, event.dx, event.dy);
}

void Application::on_offset_bound_viewport(const OffsetBoundViewportEvent& event)
{
  auto& registry = mModel.get_active_registry();
  sys::OffsetBoundViewport(registry,
                           event.entity,
                           event.dx,
                           event.dy,
                           event.view_width,
                           event.view_height);
}

void Application::on_pan_left()
{
  auto& registry = mModel.get_active_registry();
  sys::PanViewportLeft(registry);
}

void Application::on_pan_right()
{
  auto& registry = mModel.get_active_registry();
  sys::PanViewportRight(registry);
}

void Application::on_pan_up()
{
  auto& registry = mModel.get_active_registry();
  sys::PanViewportUp(registry);
}

void Application::on_pan_down()
{
  auto& registry = mModel.get_active_registry();
  sys::PanViewportDown(registry);
}

void Application::on_increase_zoom()
{
  auto& registry = mModel.get_active_registry();
  const auto mousePos = ImGui::GetIO().MousePos;
  sys::IncreaseViewportZoom(registry, mousePos.x, mousePos.y);
}

void Application::on_decrease_zoom()
{
  auto& registry = mModel.get_active_registry();
  const auto mousePos = ImGui::GetIO().MousePos;
  sys::DecreaseViewportZoom(registry, mousePos.x, mousePos.y);
}

void Application::on_reset_zoom()
{
  auto& registry = mModel.get_active_registry();
  sys::ResetViewportZoom(registry);
}

void Application::on_show_tileset_creation_dialog()
{
  mWidgets.show_add_tileset_dialog();
}

void Application::on_add_tileset(const AddTilesetEvent& event)
{
  if (auto info = mTextures.load(event.path)) {
    _execute<AddTilesetCmd>(mModel,
                            std::move(*info),
                            event.tile_width,
                            event.tile_height);
  }
  else {
    log_error("Failed to load tileset texture!");
  }
}

void Application::on_remove_tileset(const RemoveTilesetEvent& event)
{
  _execute<RemoveTilesetCmd>(mModel, event.id);
}

void Application::on_select_tileset(const SelectTilesetEvent& event)
{
  auto& registry = mModel.get_active_registry();
  sys::select_tileset(registry, event.id);
}

void Application::on_set_tileset_selection(const SetTilesetSelectionEvent& event)
{
  auto& registry = mModel.get_active_registry();
  sys::update_tileset_selection(registry, event.selection);
}

void Application::on_set_tileset_name(const SetTilesetNameEvent& event)
{
  _execute<RenameTilesetCmd>(mModel, event.id, event.name);
}

void Application::on_add_row()
{
  _execute<AddRowCmd>(mModel);
}

void Application::on_add_column()
{
  _execute<AddColumnCmd>(mModel);
}

void Application::on_remove_row()
{
  _execute<RemoveRowCmd>(mModel);
}

void Application::on_remove_column()
{
  _execute<RemoveColumnCmd>(mModel);
}

void Application::on_resize_map(const ResizeMapEvent& event)
{
  _execute<ResizeMapCmd>(mModel, event.row_count, event.col_count);
}

void Application::on_open_resize_map_dialog()
{
  if (auto* registry = mModel.active_registry()) {
    const auto& map = registry->ctx<MapInfo>();
    mWidgets.show_resize_map_dialog(map.row_count, map.column_count);
  }
}

void Application::on_add_layer(const AddLayerEvent& event)
{
  _execute<AddLayerCmd>(mModel, event.type);
}

void Application::on_remove_layer(const RemoveLayerEvent& event)
{
  _execute<RemoveLayerCmd>(mModel, event.id);
}

void Application::on_select_layer(const SelectLayerEvent& event)
{
  if (auto* registry = mModel.active_registry()) {
    auto& active = registry->ctx<comp::ActiveLayer>();
    active.entity = sys::find_layer(*registry, event.id);
  }
}

void Application::on_move_layer_up(const MoveLayerUpEvent& event)
{
  _execute<MoveLayerUpCmd>(mModel, event.id);
}

void Application::on_move_layer_down(const MoveLayerDownEvent& event)
{
  _execute<MoveLayerDownCmd>(mModel, event.id);
}

void Application::on_duplicate_layer(const DuplicateLayerEvent& event)
{
  _execute<DuplicateLayerCmd>(mModel, event.id);
}

void Application::on_set_layer_opacity(const SetLayerOpacityEvent& event)
{
  _execute<SetLayerOpacityCmd>(mModel, event.id, event.opacity);
}

void Application::on_set_layer_visible(const SetLayerVisibleEvent& event)
{
  _execute<SetLayerVisibilityCmd>(mModel, event.id, event.visible);
}

void Application::on_open_rename_layer_dialog(const OpenRenameLayerDialogEvent& event)
{
  mWidgets.show_rename_layer_dialog(event.id);
}

void Application::on_rename_layer(const RenameLayerEvent& event)
{
  _execute<RenameLayerCmd>(mModel, event.id, event.name);
}

void Application::on_set_object_name(const SetObjectNameEvent& event)
{
  _execute<SetObjectNameCmd>(mModel, event.id, event.name);
}

void Application::on_move_object(const MoveObjectEvent& event)
{
  _register<MoveObjectCmd>(mModel,
                           event.id,
                           event.old_x,
                           event.old_y,
                           event.new_x,
                           event.new_y);
}

void Application::on_set_object_visibility(const SetObjectVisibilityEvent& event)
{
  _execute<SetObjectVisibilityCmd>(mModel, event.id, event.visible);
}

void Application::on_set_object_tag(const SetObjectTagEvent& event)
{
  _execute<SetObjectTagCmd>(mModel, event.id, event.tag);
}

void Application::on_spawn_object_context_menu(const SpawnObjectContextMenuEvent&)
{
  open_object_context_menu();
}

void Application::on_show_add_property_dialog()
{
  mWidgets.show_add_property_dialog();
}

void Application::on_show_rename_property_dialog(
    const ShowRenamePropertyDialogEvent& event)
{
  mWidgets.show_rename_property_dialog(event.name);
}

void Application::on_show_change_property_type_dialog(
    const ShowChangePropertyTypeDialogEvent& event)
{
  mWidgets.show_change_property_type_dialog(event.name, event.current_type);
}

void Application::on_add_property(const AddPropertyEvent& event)
{
  _execute<AddPropertyCmd>(mModel, event.name, event.type);
}

void Application::on_remove_property(const RemovePropertyEvent& event)
{
  _execute<RemovePropertyCmd>(mModel, event.name);
}

void Application::on_rename_property(const RenamePropertyEvent& event)
{
  _execute<RenamePropertyCmd>(mModel, event.old_name, event.new_name);
}

void Application::on_update_property(const UpdatePropertyEvent& event)
{
  _execute<UpdatePropertyCmd>(mModel, event.name, event.value);
}

void Application::on_change_property_type(const ChangePropertyTypeEvent& event)
{
  _execute<ChangePropertyTypeCmd>(mModel, event.name, event.type);
}

void Application::on_inspect_context(const InspectContextEvent& event)
{
  auto& registry = mModel.get_active_registry();
  auto& current = registry.ctx<comp::ActiveAttributeContext>();
  current.entity = event.entity;
}

void Application::on_open_component_editor()
{
  mWidgets.show_component_editor(mModel);
}

void Application::on_create_component_def(const CreateComponentDefEvent& event)
{
  _execute<CreateComponentDefCmd>(mModel, event.name);
}

void Application::on_remove_component_def(const RemoveComponentDefEvent& event)
{
  _execute<RemoveComponentDefCmd>(mModel, event.id);
}

void Application::on_rename_component_def(const RenameComponentDefEvent& event)
{
  _execute<RenameComponentCmd>(mModel, event.id, event.name);
}

void Application::on_create_component_attr(const CreateComponentAttrEvent& event)
{
  _execute<CreateComponentAttrCmd>(mModel, event.id, event.name);
}

void Application::on_remove_component_attr(const RemoveComponentAttrEvent& event)
{
  _execute<RemoveComponentAttrCmd>(mModel, event.id, event.name);
}

void Application::on_rename_component_attr(const RenameComponentAttrEvent& event)
{
  _execute<RenameComponentAttrCmd>(mModel, event.id, event.previous, event.updated);
}

void Application::on_duplicate_component_attr(const DuplicateComponentAttrEvent& event)
{
  _execute<DuplicateComponentAttrCmd>(mModel, event.id, event.attribute);
}

void Application::on_set_component_attr_type(const SetComponentAttrTypeEvent& event)
{
  _execute<SetComponentAttrTypeCmd>(mModel, event.id, event.attribute, event.type);
}

void Application::on_update_component_def_attr(const UpdateComponentDefAttrEvent& event)
{
  _execute<UpdateComponentAttrCmd>(mModel, event.id, event.attribute, event.value);
}

void Application::on_add_component(const AddComponentEvent& event)
{
  _execute<AddComponentCmd>(mModel, event.context, event.component);
}

void Application::on_remove_component(const RemoveComponentEvent& event)
{
  _execute<RemoveComponentCmd>(mModel, event.context, event.component);
}

void Application::on_update_component(const UpdateComponentEvent& event)
{
  _execute<UpdateComponentCmd>(mModel,
                               event.context,
                               event.component,
                               event.attribute,
                               event.value);
}

void Application::on_reset_component_values(const ResetComponentValuesEvent& event)
{
  _execute<ResetComponentCmd>(mModel, event.context, event.component);
}

void Application::on_toggle_ui()
{
  if (ImGui::GetTopMostPopupModal() != nullptr) {
    return;
  }

  static bool show = false;
  auto& prefs = get_preferences();

  if (!show) {
    mWidgetShowState.prev_show_layer_dock = prefs.is_layer_dock_visible();
    mWidgetShowState.prev_show_tileset_dock = prefs.is_tileset_dock_visible();
    mWidgetShowState.prev_show_property_dock = prefs.is_property_dock_visible();
    mWidgetShowState.prev_show_log_dock = prefs.is_log_dock_visible();
    mWidgetShowState.prev_show_component_dock = prefs.is_component_dock_visible();
    mWidgetShowState.prev_show_toolbar = mWidgets.is_toolbar_visible();
  }

  prefs.set_layer_dock_visible(show);
  prefs.set_tileset_dock_visible(show);
  prefs.set_property_dock_visible(show);
  prefs.set_component_dock_visible(show);
  prefs.set_log_dock_visible(show);

  mWidgets.set_toolbar_visible(show);

  if (show) {
    prefs.set_layer_dock_visible(mWidgetShowState.prev_show_layer_dock);
    prefs.set_tileset_dock_visible(mWidgetShowState.prev_show_tileset_dock);
    prefs.set_property_dock_visible(mWidgetShowState.prev_show_property_dock);
    prefs.set_log_dock_visible(mWidgetShowState.prev_show_log_dock);
    prefs.set_component_dock_visible(mWidgetShowState.prev_show_component_dock);

    mWidgets.set_toolbar_visible(mWidgetShowState.prev_show_toolbar);
  }

  show = !show;
}

void Application::on_quit()
{
  mQuit = true;
}

}  // namespace tactile
