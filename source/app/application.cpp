#include "application.hpp"

#include <utility>  // move, forward

#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <imgui_internal.h>

#include "application_events.hpp"
#include "cfg/configuration.hpp"
#include "core/components/attribute_context.hpp"
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
void _execute(document_model& model, Args&&... args)
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
void _register(document_model& model, Args&&... args)
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

application::application(app_configuration* configuration)
    : mConfiguration{configuration}
    , mIcons{mTextures}
{
  subscribe_to_events(*this);
  LoadDefaultShortcuts();
}

auto application::run() -> int
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

void application::on_about_to_exit()
{
  save_current_files_to_history();
  save_preferences();
  save_session(mModel);
  save_file_history();
}

void application::save_current_files_to_history()
{
  for (const auto& [id, document] : mModel) {
    if (!document->path.empty()) {
      add_file_to_history(document->path);
    }
  }
}

void application::poll_events()
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

void application::on_keyboard_event(cen::keyboard_event event)
{
  /* We don't care about these modifiers, they are just noise */
  event.set_modifier(cen::key_mod::caps, false);
  event.set_modifier(cen::key_mod::num, false);
  event.set_modifier(cen::key_mod::mode, false);

  UpdateShortcuts(mModel, mWidgets, event, mDispatcher);
}

void application::on_mouse_wheel_event(const cen::mouse_wheel_event& event)
{
  constexpr float scaling = 4.0f;

  const auto* registry = mModel.active_registry();
  if (registry && !ImGui::GetTopMostPopupModal()) {
    if (IsMouseWithinViewport()) {
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

void application::update_frame()
{
  mDispatcher.update();
  mModel.update();
  mWidgets.update(mModel, mIcons, mDispatcher);
}

void application::on_undo()
{
  if (auto* document = mModel.active_document()) {
    document->commands.undo();
  }
}

void application::on_redo()
{
  if (auto* document = mModel.active_document()) {
    document->commands.redo();
  }
}

void application::on_set_command_capacity(const set_command_capacity_event& event)
{
  mModel.set_command_capacity(event.capacity);
}

void application::on_save()
{
  if (auto* document = mModel.active_document()) {
    if (!document->path.empty()) {
      save_document(*document);
      document->commands.mark_as_clean();

      auto& context = document->registry.ctx<comp::attribute_context>();
      context.name = document->path.filename().string();
    }
    else {
      on_open_save_as_dialog();
    }
  }
}

void application::on_save_as(const save_as_event& event)
{
  if (auto* document = mModel.active_document()) {
    document->path = event.path;
    on_save();
  }
}

void application::on_open_save_as_dialog()
{
  if (mModel.has_active_document()) {
    show_save_as_dialog(mDispatcher);
  }
}

void application::on_show_settings()
{
  mWidgets.show_settings();
}

void application::on_show_new_map_dialog()
{
  mWidgets.show_new_map_dialog();
}

void application::on_show_open_map_dialog()
{
  mWidgets.show_open_map_dialog();
}

void application::on_show_map_properties()
{
  if (auto* registry = mModel.active_registry()) {
    auto& current = registry->ctx<comp::active_attribute_context>();
    current.entity = entt::null;
  }
}

void application::on_create_map(const create_map_event& event)
{
  const auto id = mModel.add_map(event.tile_width,
                                 event.tile_height,
                                 event.row_count,
                                 event.column_count);
  mModel.select_map(id);
}

void application::on_close_map(const close_map_event& event)
{
  if (mModel.has_path(event.id)) {
    set_last_closed_file(mModel.get_path(event.id));
  }
  mModel.remove_map(event.id);
}

void application::on_open_map(const open_map_event& event)
{
  /* Just silently ignore the request if the map is already open */
  if (mModel.has_document_with_path(event.path)) {
    log_warning("Tried to open map that was already open!");
    return;
  }

  const auto ir = parsing::parse_map(event.path);
  if (ir.error() == parsing::parse_error::none) {
    mModel.add_map(restore_document_from_ir(ir, mTextures));
    add_file_to_history(event.path);
  }
  else {
    mWidgets.show_map_import_error_dialog(ir.error());
  }
}

void application::on_select_map(const select_map_event& event)
{
  mModel.select_map(event.id);
}

void application::on_select_tool(const select_tool_event& event)
{
  auto& registry = mModel.get_active_registry();
  sys::SelectTool(registry, event.type);
}

void application::on_mouse_pressed(const mouse_pressed_event& event)
{
  if (auto* registry = mModel.active_registry()) {
    sys::ToolOnPressed(*registry, mDispatcher, event.info);
  }
}

void application::on_mouse_drag(const mouse_drag_event& event)
{
  if (auto* registry = mModel.active_registry()) {
    sys::ToolOnDragged(*registry, mDispatcher, event.info);
  }
}

void application::on_mouse_released(const mouse_released_event& event)
{
  if (auto* registry = mModel.active_registry()) {
    sys::ToolOnReleased(*registry, mDispatcher, event.info);
  }
}

void application::on_stamp_sequence(stamp_sequence_event event)
{
  _register<stamp_sequence_cmd>(mModel,
                                std::move(event.old_state),
                                std::move(event.sequence));
}

void application::on_eraser_sequence(eraser_sequence_event event)
{
  _register<eraser_sequence_cmd>(mModel, std::move(event.old_state));
}

void application::on_flood(const flood_event& event)
{
  _execute<bucket_cmd>(mModel, event.origin, event.replacement);
}

void application::on_center_viewport()
{
  CenterViewport();
}

void application::on_offset_viewport(const OffsetViewportEvent& event)
{
  auto& registry = mModel.get_active_registry();
  sys::OffsetViewport(registry, event.dx, event.dy);
}

void application::on_offset_bound_viewport(const OffsetBoundViewportEvent& event)
{
  auto& registry = mModel.get_active_registry();
  sys::OffsetBoundViewport(registry,
                           event.entity,
                           event.dx,
                           event.dy,
                           event.view_width,
                           event.view_height);
}

void application::on_pan_left()
{
  auto& registry = mModel.get_active_registry();
  sys::PanViewportLeft(registry);
}

void application::on_pan_right()
{
  auto& registry = mModel.get_active_registry();
  sys::PanViewportRight(registry);
}

void application::on_pan_up()
{
  auto& registry = mModel.get_active_registry();
  sys::PanViewportUp(registry);
}

void application::on_pan_down()
{
  auto& registry = mModel.get_active_registry();
  sys::PanViewportDown(registry);
}

void application::on_increase_zoom()
{
  auto& registry = mModel.get_active_registry();
  const auto mousePos = ImGui::GetIO().MousePos;
  sys::IncreaseViewportZoom(registry, mousePos.x, mousePos.y);
}

void application::on_decrease_zoom()
{
  auto& registry = mModel.get_active_registry();
  const auto mousePos = ImGui::GetIO().MousePos;
  sys::DecreaseViewportZoom(registry, mousePos.x, mousePos.y);
}

void application::on_reset_zoom()
{
  auto& registry = mModel.get_active_registry();
  sys::ResetViewportZoom(registry);
}

void application::on_show_add_tileset_dialog()
{
  mWidgets.show_add_tileset_dialog();
}

void application::on_add_tileset(const add_tileset_event& event)
{
  if (auto info = mTextures.load(event.path)) {
    _execute<add_tileset_cmd>(mModel,
                              std::move(*info),
                              event.tile_width,
                              event.tile_height);
  }
  else {
    log_error("Failed to load tileset texture!");
  }
}

void application::on_remove_tileset(const remove_tileset_event& event)
{
  _execute<remove_tileset_cmd>(mModel, event.id);
}

void application::on_select_tileset(const select_tileset_event& event)
{
  auto& registry = mModel.get_active_registry();
  sys::select_tileset(registry, event.id);
}

void application::on_set_tileset_selection(const set_tileset_selection_event& event)
{
  auto& registry = mModel.get_active_registry();
  sys::update_tileset_selection(registry, event.selection);
}

void application::on_set_tileset_name(const set_tileset_name_event& event)
{
  _execute<set_tileset_name_cmd>(mModel, event.id, event.name);
}

void application::on_add_row()
{
  _execute<add_row_cmd>(mModel);
}

void application::on_add_column()
{
  _execute<add_column_cmd>(mModel);
}

void application::on_remove_row()
{
  _execute<remove_row_cmd>(mModel);
}

void application::on_remove_column()
{
  _execute<remove_column_cmd>(mModel);
}

void application::on_resize_map(const resize_map_event& event)
{
  _execute<resize_map_cmd>(mModel, event.row_count, event.col_count);
}

void application::on_open_resize_map_dialog()
{
  if (auto* registry = mModel.active_registry()) {
    const auto& map = registry->ctx<MapInfo>();
    mWidgets.show_resize_map_dialog(map.row_count, map.column_count);
  }
}

void application::on_add_layer(const add_layer_event& event)
{
  _execute<add_layer_cmd>(mModel, event.type);
}

void application::on_remove_layer(const remove_layer_event& event)
{
  _execute<remove_layer_cmd>(mModel, event.id);
}

void application::on_select_layer(const select_layer_event& event)
{
  if (auto* registry = mModel.active_registry()) {
    auto& active = registry->ctx<comp::active_layer>();
    active.entity = sys::find_layer(*registry, event.id);
  }
}

void application::on_move_layer_up(const move_layer_up_event& event)
{
  _execute<move_layer_up_cmd>(mModel, event.id);
}

void application::on_move_layer_down(const move_layer_down_event& event)
{
  _execute<move_layer_down_cmd>(mModel, event.id);
}

void application::on_duplicate_layer(const duplicate_layer_event& event)
{
  _execute<duplicate_layer_cmd>(mModel, event.id);
}

void application::on_set_layer_opacity(const set_layer_opacity_event& event)
{
  _execute<set_layer_opacity_cmd>(mModel, event.id, event.opacity);
}

void application::on_set_layer_visible(const set_layer_visible_event& event)
{
  _execute<set_layer_visibility_cmd>(mModel, event.id, event.visible);
}

void application::on_open_rename_layer_dialog(const open_rename_layer_dialog_event& event)
{
  mWidgets.show_rename_layer_dialog(event.id);
}

void application::on_rename_layer(const rename_layer_event& event)
{
  _execute<rename_layer_cmd>(mModel, event.id, event.name);
}

void application::on_set_object_name(const set_object_name_event& event)
{
  _execute<set_object_name_cmd>(mModel, event.id, event.name);
}

void application::on_move_object(const move_object_event& event)
{
  _register<move_object_cmd>(mModel,
                             event.id,
                             event.old_x,
                             event.old_y,
                             event.new_x,
                             event.new_y);
}

void application::on_set_object_visibility(const set_object_visibility_event& event)
{
  _execute<set_object_visibility_cmd>(mModel, event.id, event.visible);
}

void application::on_set_object_tag(const set_object_tag_event& event)
{
  _execute<set_object_tag_cmd>(mModel, event.id, event.tag);
}

void application::on_spawn_object_context_menu(const spawn_object_context_menu_event&)
{
  OpenObjectContextMenu();
}

void application::on_show_add_property_dialog()
{
  mWidgets.show_add_property_dialog();
}

void application::on_show_rename_property_dialog(
    const show_rename_property_dialog_event& event)
{
  mWidgets.show_rename_property_dialog(event.name);
}

void application::on_show_change_property_type_dialog(
    const show_change_property_type_dialog_event& event)
{
  mWidgets.show_change_property_type_dialog(event.name, event.current_type);
}

void application::on_add_property(const add_property_event& event)
{
  _execute<add_property_cmd>(mModel, event.name, event.type);
}

void application::on_remove_property(const remove_property_event& event)
{
  _execute<remove_property_cmd>(mModel, event.name);
}

void application::on_rename_property(const rename_property_event& event)
{
  _execute<rename_property_cmd>(mModel, event.old_name, event.new_name);
}

void application::on_update_property(const update_property_event& event)
{
  _execute<update_property_cmd>(mModel, event.name, event.value);
}

void application::on_change_property_type(const change_property_type_event& event)
{
  _execute<change_property_type_cmd>(mModel, event.name, event.type);
}

void application::on_inspect_context(const inspect_context_event& event)
{
  auto& registry = mModel.get_active_registry();
  auto& current = registry.ctx<comp::active_attribute_context>();
  current.entity = event.entity;
}

void application::on_open_component_editor()
{
  mWidgets.show_component_editor(mModel);
}

void application::on_create_component_def(const create_component_def_event& event)
{
  _execute<create_component_def_cmd>(mModel, event.name);
}

void application::on_remove_component_def(const remove_component_def_event& event)
{
  _execute<remove_component_def_cmd>(mModel, event.id);
}

void application::on_rename_component_def(const rename_component_def_event& event)
{
  _execute<rename_component_cmd>(mModel, event.id, event.name);
}

void application::on_create_component_attr(const create_component_attr_event& event)
{
  _execute<create_component_attr_cmd>(mModel, event.id, event.name);
}

void application::on_remove_component_attr(const remove_component_attr_event& event)
{
  _execute<remove_component_attr_cmd>(mModel, event.id, event.name);
}

void application::on_rename_component_attr(const rename_component_attr_event& event)
{
  _execute<rename_component_attr_cmd>(mModel, event.id, event.previous, event.updated);
}

void application::on_duplicate_component_attr(const duplicate_component_attr_event& event)
{
  _execute<duplicate_component_attr_cmd>(mModel, event.id, event.attribute);
}

void application::on_set_component_attr_type(const set_component_attr_type_event& event)
{
  _execute<set_component_attr_type_cmd>(mModel, event.id, event.attribute, event.type);
}

void application::on_update_component_def_attr(
    const update_component_def_attr_event& event)
{
  _execute<update_component_attr_cmd>(mModel, event.id, event.attribute, event.value);
}

void application::on_add_component(const add_component_event& event)
{
  _execute<add_component_cmd>(mModel, event.context, event.component);
}

void application::on_remove_component(const remove_component_event& event)
{
  _execute<remove_component_cmd>(mModel, event.context, event.component);
}

void application::on_update_component(const update_component_event& event)
{
  _execute<update_component_cmd>(mModel,
                                 event.context,
                                 event.component,
                                 event.attribute,
                                 event.value);
}

void application::on_reset_component_values(const reset_component_values_event& event)
{
  _execute<reset_component_cmd>(mModel, event.context, event.component);
}

void application::on_toggle_ui()
{
  if (ImGui::GetTopMostPopupModal() != nullptr) {
    return;
  }

  static bool show = false;
  auto& prefs = get_preferences();

  if (!show) {
    mWidgetShowState.prev_show_layer_dock = prefs.is_layer_dock_visible();
    mWidgetShowState.prev_show_tileset_dock = prefs.is_tileset_dock_visible();
    mWidgetShowState.prev_show_properties_dock = prefs.is_properties_dock_visible();
    mWidgetShowState.prev_show_log_dock = prefs.is_log_dock_visible();
    mWidgetShowState.prev_show_component_dock = prefs.is_component_dock_visible();
    mWidgetShowState.prev_show_toolbar = mWidgets.is_toolbar_visible();
  }

  prefs.set_layer_dock_visible(show);
  prefs.set_tileset_dock_visible(show);
  prefs.set_properties_dock_visible(show);
  prefs.set_component_dock_visible(show);
  prefs.set_log_dock_visible(show);

  mWidgets.set_toolbar_visible(show);

  if (show) {
    prefs.set_layer_dock_visible(mWidgetShowState.prev_show_layer_dock);
    prefs.set_tileset_dock_visible(mWidgetShowState.prev_show_tileset_dock);
    prefs.set_properties_dock_visible(mWidgetShowState.prev_show_properties_dock);
    prefs.set_log_dock_visible(mWidgetShowState.prev_show_log_dock);
    prefs.set_component_dock_visible(mWidgetShowState.prev_show_component_dock);

    mWidgets.set_toolbar_visible(mWidgetShowState.prev_show_toolbar);
  }

  show = !show;
}

void application::on_quit()
{
  mQuit = true;
}

}  // namespace tactile
