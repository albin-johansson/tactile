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

#include <entt/entt.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "application_events.hpp"
#include "cfg/configuration.hpp"
#include "cfg/fonts.hpp"
#include "core/components/attributes.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tools/tool_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "core/utils/texture_manager.hpp"
#include "core/viewport.hpp"
#include "editor/commands/commands.hpp"
#include "editor/gui/dialogs/save_as_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/menus/edit_menu.hpp"
#include "editor/gui/viewport/map_view.hpp"
#include "editor/gui/viewport/viewport_widget.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "editor/shortcuts/shortcuts.hpp"
#include "io/maps/parser/parse_map.hpp"
#include "io/maps/restore_document_from_ir.hpp"
#include "io/maps/save_document.hpp"
#include "io/persistence/history.hpp"
#include "io/persistence/preferences.hpp"
#include "io/persistence/session.hpp"
#include "misc/assert.hpp"
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

/**
 * \brief Tracks visibility of widgets for the "Toggle UI" feature.
 */
struct WidgetShowState final
{
  bool prev_show_layer_dock{};
  bool prev_show_tileset_dock{};
  bool prev_show_property_dock{};
  bool prev_show_log_dock{};
  bool prev_show_component_dock{};
};

struct Application::Data final
{
  AppConfiguration* config{}; /* Non-owning */
  entt::dispatcher dispatcher;
  DocumentModel model;
  TextureManager textures;
  WidgetManager widgets;
  WidgetShowState widget_show_state;
  bool reload_fonts{};
};

Application::Application(AppConfiguration* configuration)
    : AEventLoop{configuration}
    , mData{std::make_unique<Data>()}
{
  mData->config = configuration;

  subscribe_to_events(this);
  load_default_shortcuts();
  load_icons(mData->textures);
}

Application::~Application() noexcept = default;

void Application::on_startup()
{
  load_file_history();

  if (get_preferences().will_restore_last_session()) {
    restore_last_session(mData->model, mData->textures);
  }

  auto& window = mData->config->window();
  window.show();
}

void Application::on_shutdown()
{
  save_current_files_to_history();
  save_preferences();
  save_session(mData->model);
  save_file_history();

  auto& window = mData->config->window();
  window.hide();
}

void Application::on_pre_update()
{
  if (mData->reload_fonts) {
    reload_fonts();
    mData->reload_fonts = false;
  }
}

void Application::on_update()
{
  auto& data = *mData;
  data.dispatcher.update();
  data.model.update();
  data.widgets.update(data.model, data.dispatcher);
}

void Application::on_event(const cen::event_handler& handler)
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

auto Application::get_dispatcher() -> entt::dispatcher&
{
  return mData->dispatcher;
}

void Application::save_current_files_to_history()
{
  for (const auto& [id, document] : mData->model) {
    if (!document->path.empty()) {
      add_file_to_history(document->path);
    }
  }
}

void Application::on_keyboard_event(cen::keyboard_event event)
{
  /* We don't care about these modifiers, they are just noise */
  event.set_modifier(cen::key_mod::caps, false);
  event.set_modifier(cen::key_mod::num, false);
  event.set_modifier(cen::key_mod::mode, false);

  auto& data = *mData;
  update_shortcuts(data.model, data.widgets, event, data.dispatcher);
}

void Application::on_mouse_wheel_event(const cen::mouse_wheel_event& event)
{
  constexpr float scaling = 4.0f;

  auto& data = *mData;
  const auto* registry = data.model.active_registry();

  if (registry && !ImGui::GetTopMostPopupModal()) {
    if (is_mouse_within_viewport()) {
      const auto& viewport = registry->ctx<Viewport>();
      if (cen::is_active(primary_modifier)) {
        const auto y = event.precise_y();
        if (y > 0) {
          data.dispatcher.enqueue<IncreaseZoomEvent>();
        }
        else if (y < 0 && sys::can_decrease_viewport_zoom(*registry)) {
          data.dispatcher.enqueue<DecreaseZoomEvent>();
        }
      }
      else {
        const auto dx = event.precise_x() * (viewport.tile_width / scaling);
        const auto dy = event.precise_y() * (viewport.tile_height / scaling);
        data.dispatcher.enqueue<OffsetViewportEvent>(-dx, dy);
      }
    }
    else if (data.widgets.is_tileset_dock_hovered()) {
      const auto width = data.widgets.tileset_view_width();
      const auto height = data.widgets.tileset_view_height();
      if (width && height) {
        const auto entity = sys::find_active_tileset(*registry);
        TACTILE_ASSERT(entity != entt::null);

        const auto& viewport = registry->get<Viewport>(entity);

        const auto dx = event.precise_x() * (viewport.tile_width / scaling);
        const auto dy = event.precise_y() * (viewport.tile_height / scaling);
        data.dispatcher.enqueue<OffsetBoundViewportEvent>(entity,
                                                          -dx,
                                                          dy,
                                                          *width,
                                                          *height);
      }
    }
  }
}

void Application::on_undo()
{
  if (auto* document = mData->model.active_document()) {
    document->commands.undo();
  }
}

void Application::on_redo()
{
  if (auto* document = mData->model.active_document()) {
    document->commands.redo();
  }
}

void Application::on_set_command_capacity(const SetCommandCapacityEvent& event)
{
  mData->model.set_command_capacity(event.capacity);
}

void Application::on_save()
{
  if (auto* document = mData->model.active_document()) {
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
  if (auto* document = mData->model.active_document()) {
    document->path = event.path;
    on_save();
  }
}

void Application::on_open_save_as_dialog()
{
  if (mData->model.has_active_document()) {
    show_save_as_dialog(mData->dispatcher);
  }
}

void Application::on_show_settings()
{
  show_settings_dialog();
}

void Application::on_show_new_map_dialog()
{
  mData->widgets.show_new_map_dialog();
}

void Application::on_show_open_map_dialog()
{
  mData->widgets.show_open_map_dialog();
}

void Application::on_show_map_properties()
{
  if (auto* registry = mData->model.active_registry()) {
    auto& current = registry->ctx<comp::ActiveAttributeContext>();
    current.entity = entt::null;
  }
}

void Application::on_create_map(const CreateMapEvent& event)
{
  const auto id = mData->model.add_map(event.tile_width,
                                       event.tile_height,
                                       event.row_count,
                                       event.column_count);
  mData->model.select_map(id);
}

void Application::on_close_map(const CloseMapEvent& event)
{
  auto& model = mData->model;
  if (model.has_path(event.id)) {
    set_last_closed_file(model.get_path(event.id));
  }
  model.remove_map(event.id);
}

void Application::on_open_map(const OpenMapEvent& event)
{
  /* Just silently ignore the request if the map is already open */
  if (mData->model.has_document_with_path(event.path)) {
    log_warning("Tried to open map that was already open!");
    return;
  }

  const auto ir = parsing::parse_map(event.path);
  if (ir.error() == parsing::ParseError::none) {
    mData->model.add_map(restore_document_from_ir(ir, mData->textures));
    add_file_to_history(event.path);
  }
  else {
    mData->widgets.show_map_import_error_dialog(ir.error());
  }
}

void Application::on_select_map(const SelectMapEvent& event)
{
  mData->model.select_map(event.id);
}

void Application::on_select_tool(const SelectToolEvent& event)
{
  auto& registry = mData->model.get_active_registry();
  sys::select_tool(registry, mData->dispatcher, event.type);
}

void Application::on_tool_pressed(const ToolPressedEvent& event)
{
  if (auto* registry = mData->model.active_registry()) {
    sys::on_tool_pressed(*registry, mData->dispatcher, event.info);
  }
}

void Application::on_tool_dragged(const ToolDraggedEvent& event)
{
  if (auto* registry = mData->model.active_registry()) {
    sys::on_tool_dragged(*registry, mData->dispatcher, event.info);
  }
}

void Application::on_tool_released(const ToolReleasedEvent& event)
{
  if (auto* registry = mData->model.active_registry()) {
    sys::on_tool_released(*registry, mData->dispatcher, event.info);
  }
}

void Application::on_tool_entered()
{
  if (auto* registry = mData->model.active_registry()) {
    sys::on_tool_entered(*registry, mData->dispatcher);
  }
}

void Application::on_tool_exited()
{
  if (auto* registry = mData->model.active_registry()) {
    sys::on_tool_exited(*registry, mData->dispatcher);
  }
}

void Application::on_stamp_sequence(StampSequenceEvent event)
{
  _register<StampToolCmd>(mData->model,
                          std::move(event.old_state),
                          std::move(event.sequence));
}

void Application::on_eraser_sequence(EraserSequenceEvent event)
{
  _register<EraserToolCmd>(mData->model, std::move(event.old_state));
}

void Application::on_flood(const FloodEvent& event)
{
  _execute<BucketToolCmd>(mData->model, event.origin, event.replacement);
}

void Application::on_add_rectangle(const AddRectangleEvent& event)
{
  _execute<RectangleToolCmd>(mData->model, event.x, event.y, event.width, event.height);
}

void Application::on_add_ellipse(const AddEllipseEvent& event)
{
  _execute<EllipseToolCmd>(mData->model, event.x, event.y, event.width, event.height);
}

void Application::on_add_point(const AddPointEvent& event)
{
  _execute<PointToolCmd>(mData->model, event.x, event.y);
}

void Application::on_center_viewport()
{
  center_map_viewport();
}

void Application::on_offset_viewport(const OffsetViewportEvent& event)
{
  auto& registry = mData->model.get_active_registry();
  sys::offset_viewport(registry, event.dx, event.dy);
}

void Application::on_offset_bound_viewport(const OffsetBoundViewportEvent& event)
{
  auto& registry = mData->model.get_active_registry();
  sys::offset_bound_viewport(registry,
                             event.entity,
                             event.dx,
                             event.dy,
                             event.view_width,
                             event.view_height);
}

void Application::on_pan_left()
{
  auto& registry = mData->model.get_active_registry();
  sys::pan_viewport_left(registry);
}

void Application::on_pan_right()
{
  auto& registry = mData->model.get_active_registry();
  sys::pan_viewport_right(registry);
}

void Application::on_pan_up()
{
  auto& registry = mData->model.get_active_registry();
  sys::pan_viewport_up(registry);
}

void Application::on_pan_down()
{
  auto& registry = mData->model.get_active_registry();
  sys::pan_viewport_down(registry);
}

void Application::on_increase_zoom()
{
  auto& registry = mData->model.get_active_registry();
  const auto mousePos = ImGui::GetIO().MousePos;
  sys::increase_viewport_zoom(registry, mousePos.x, mousePos.y);
}

void Application::on_decrease_zoom()
{
  auto& registry = mData->model.get_active_registry();
  const auto mousePos = ImGui::GetIO().MousePos;
  sys::decrease_viewport_zoom(registry, mousePos.x, mousePos.y);
}

void Application::on_reset_zoom()
{
  auto& registry = mData->model.get_active_registry();
  sys::reset_viewport_zoom(registry);
}

void Application::on_reset_font_size()
{
  get_preferences().set_font_size(get_default_font_size());
  mData->reload_fonts = true;
}

void Application::on_increase_font_size()
{
  auto& prefs = get_preferences();

  TACTILE_ASSERT(prefs.font_size() + 2 <= get_max_font_size());
  prefs.set_font_size(prefs.font_size() + 2);

  mData->reload_fonts = true;
}

void Application::on_decrease_font_size()
{
  auto& prefs = get_preferences();

  TACTILE_ASSERT(prefs.font_size() - 2 >= get_min_font_size());
  prefs.set_font_size(prefs.font_size() - 2);

  mData->reload_fonts = true;
}

void Application::on_show_tileset_creation_dialog()
{
  mData->widgets.show_add_tileset_dialog();
}

void Application::on_add_tileset(const AddTilesetEvent& event)
{
  if (auto info = mData->textures.load(event.path)) {
    _execute<AddTilesetCmd>(mData->model,
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
  _execute<RemoveTilesetCmd>(mData->model, event.id);
}

void Application::on_select_tileset(const SelectTilesetEvent& event)
{
  auto& registry = mData->model.get_active_registry();
  sys::select_tileset(registry, event.id);
}

void Application::on_set_tileset_selection(const SetTilesetSelectionEvent& event)
{
  auto& registry = mData->model.get_active_registry();
  sys::update_tileset_selection(registry, event.selection);
}

void Application::on_set_tileset_name(const SetTilesetNameEvent& event)
{
  _execute<RenameTilesetCmd>(mData->model, event.id, event.name);
}

void Application::on_add_row()
{
  _execute<AddRowCmd>(mData->model);
}

void Application::on_add_column()
{
  _execute<AddColumnCmd>(mData->model);
}

void Application::on_remove_row()
{
  _execute<RemoveRowCmd>(mData->model);
}

void Application::on_remove_column()
{
  _execute<RemoveColumnCmd>(mData->model);
}

void Application::on_resize_map(const ResizeMapEvent& event)
{
  _execute<ResizeMapCmd>(mData->model, event.row_count, event.col_count);
}

void Application::on_open_resize_map_dialog()
{
  if (auto* registry = mData->model.active_registry()) {
    const auto& map = registry->ctx<MapInfo>();
    mData->widgets.show_resize_map_dialog(map.row_count, map.column_count);
  }
}

void Application::on_add_layer(const AddLayerEvent& event)
{
  _execute<AddLayerCmd>(mData->model, event.type);
}

void Application::on_remove_layer(const RemoveLayerEvent& event)
{
  _execute<RemoveLayerCmd>(mData->model, event.id);
}

void Application::on_select_layer(const SelectLayerEvent& event)
{
  if (auto* registry = mData->model.active_registry()) {
    auto& active = registry->ctx<comp::ActiveLayer>();
    active.entity = sys::find_layer(*registry, event.id);
  }
}

void Application::on_move_layer_up(const MoveLayerUpEvent& event)
{
  _execute<MoveLayerUpCmd>(mData->model, event.id);
}

void Application::on_move_layer_down(const MoveLayerDownEvent& event)
{
  _execute<MoveLayerDownCmd>(mData->model, event.id);
}

void Application::on_duplicate_layer(const DuplicateLayerEvent& event)
{
  _execute<DuplicateLayerCmd>(mData->model, event.id);
}

void Application::on_set_layer_opacity(const SetLayerOpacityEvent& event)
{
  _execute<SetLayerOpacityCmd>(mData->model, event.id, event.opacity);
}

void Application::on_set_layer_visible(const SetLayerVisibleEvent& event)
{
  _execute<SetLayerVisibilityCmd>(mData->model, event.id, event.visible);
}

void Application::on_open_rename_layer_dialog(const OpenRenameLayerDialogEvent& event)
{
  mData->widgets.show_rename_layer_dialog(event.id);
}

void Application::on_rename_layer(const RenameLayerEvent& event)
{
  _execute<RenameLayerCmd>(mData->model, event.id, event.name);
}

void Application::on_set_object_name(const SetObjectNameEvent& event)
{
  _execute<SetObjectNameCmd>(mData->model, event.id, event.name);
}

void Application::on_move_object(const MoveObjectEvent& event)
{
  _register<MoveObjectCmd>(mData->model,
                           event.id,
                           event.old_x,
                           event.old_y,
                           event.new_x,
                           event.new_y);
}

void Application::on_set_object_visibility(const SetObjectVisibilityEvent& event)
{
  _execute<SetObjectVisibilityCmd>(mData->model, event.id, event.visible);
}

void Application::on_set_object_tag(const SetObjectTagEvent& event)
{
  _execute<SetObjectTagCmd>(mData->model, event.id, event.tag);
}

void Application::on_spawn_object_context_menu(const SpawnObjectContextMenuEvent&)
{
  open_object_context_menu();
}

void Application::on_show_add_property_dialog()
{
  mData->widgets.show_add_property_dialog();
}

void Application::on_show_rename_property_dialog(
    const ShowRenamePropertyDialogEvent& event)
{
  mData->widgets.show_rename_property_dialog(event.name);
}

void Application::on_show_change_property_type_dialog(
    const ShowChangePropertyTypeDialogEvent& event)
{
  mData->widgets.show_change_property_type_dialog(event.name, event.current_type);
}

void Application::on_add_property(const AddPropertyEvent& event)
{
  _execute<AddPropertyCmd>(mData->model, event.name, event.type);
}

void Application::on_remove_property(const RemovePropertyEvent& event)
{
  _execute<RemovePropertyCmd>(mData->model, event.name);
}

void Application::on_rename_property(const RenamePropertyEvent& event)
{
  _execute<RenamePropertyCmd>(mData->model, event.old_name, event.new_name);
}

void Application::on_update_property(const UpdatePropertyEvent& event)
{
  _execute<UpdatePropertyCmd>(mData->model, event.name, event.value);
}

void Application::on_change_property_type(const ChangePropertyTypeEvent& event)
{
  _execute<ChangePropertyTypeCmd>(mData->model, event.name, event.type);
}

void Application::on_inspect_context(const InspectContextEvent& event)
{
  auto& registry = mData->model.get_active_registry();
  auto& current = registry.ctx<comp::ActiveAttributeContext>();
  current.entity = event.entity;
}

void Application::on_open_component_editor()
{
  show_component_editor(mData->model);
}

void Application::on_create_component_def(const CreateComponentDefEvent& event)
{
  _execute<CreateComponentDefCmd>(mData->model, event.name);
}

void Application::on_remove_component_def(const RemoveComponentDefEvent& event)
{
  _execute<RemoveComponentDefCmd>(mData->model, event.id);
}

void Application::on_rename_component_def(const RenameComponentDefEvent& event)
{
  _execute<RenameComponentCmd>(mData->model, event.id, event.name);
}

void Application::on_create_component_attr(const CreateComponentAttrEvent& event)
{
  _execute<CreateComponentAttrCmd>(mData->model, event.id, event.name);
}

void Application::on_remove_component_attr(const RemoveComponentAttrEvent& event)
{
  _execute<RemoveComponentAttrCmd>(mData->model, event.id, event.name);
}

void Application::on_rename_component_attr(const RenameComponentAttrEvent& event)
{
  _execute<RenameComponentAttrCmd>(mData->model, event.id, event.previous, event.updated);
}

void Application::on_duplicate_component_attr(const DuplicateComponentAttrEvent& event)
{
  _execute<DuplicateComponentAttrCmd>(mData->model, event.id, event.attribute);
}

void Application::on_set_component_attr_type(const SetComponentAttrTypeEvent& event)
{
  _execute<SetComponentAttrTypeCmd>(mData->model, event.id, event.attribute, event.type);
}

void Application::on_update_component_def_attr(const UpdateComponentDefAttrEvent& event)
{
  _execute<UpdateComponentAttrCmd>(mData->model, event.id, event.attribute, event.value);
}

void Application::on_add_component(const AddComponentEvent& event)
{
  _execute<AddComponentCmd>(mData->model, event.context, event.component);
}

void Application::on_remove_component(const RemoveComponentEvent& event)
{
  _execute<RemoveComponentCmd>(mData->model, event.context, event.component);
}

void Application::on_update_component(const UpdateComponentEvent& event)
{
  _execute<UpdateComponentCmd>(mData->model,
                               event.context,
                               event.component,
                               event.attribute,
                               event.value);
}

void Application::on_reset_component_values(const ResetComponentValuesEvent& event)
{
  _execute<ResetComponentCmd>(mData->model, event.context, event.component);
}

void Application::on_toggle_ui()
{
  if (ImGui::GetTopMostPopupModal() != nullptr) {
    return;
  }

  static bool show = false;

  auto& prefs = get_preferences();
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

void Application::on_reload_fonts()
{
  mData->reload_fonts = true;
}

void Application::on_quit()
{
  stop();
}

}  // namespace tactile
