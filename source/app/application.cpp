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
#include "io/maps/parser/map_parser.hpp"
#include "io/maps/restore_document_from_ir.hpp"
#include "io/maps/save_document.hpp"
#include "io/persistence/history.hpp"
#include "io/persistence/preferences.hpp"
#include "io/persistence/session.hpp"
#include "logging.hpp"

namespace tactile {
namespace {

template <typename Command, typename... Args>
void _execute(Model& model, Args&&... args)
{
  if (auto* document = model.GetActiveDocument()) {
    auto& commands = document->commands;
    commands.push<Command>(document->registry, std::forward<Args>(args)...);
  }
  else {
    log_error("Could not execute a command due to no active document!");
  }
}

template <typename Command, typename... Args>
void _register(Model& model, Args&&... args)
{
  if (auto* document = model.GetActiveDocument()) {
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

  const auto* registry = mModel.GetActiveRegistry();
  if (registry && !ImGui::GetTopMostPopupModal()) {
    if (IsMouseWithinViewport()) {
      const auto& viewport = registry->ctx<Viewport>();
      const auto dx = static_cast<float>(event.x()) * (viewport.tile_width / scaling);
      const auto dy = static_cast<float>(event.y()) * (viewport.tile_height / scaling);
      mDispatcher.enqueue<OffsetViewportEvent>(-dx, dy);
    }
    else if (mWidgets.IsTilesetDockHovered()) {
      const auto width = mWidgets.GetTilesetViewWidth();
      const auto height = mWidgets.GetTilesetViewHeight();
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
  mModel.Update();
  mWidgets.Update(mModel, mIcons, mDispatcher);
}

void application::on_undo()
{
  if (auto* document = mModel.GetActiveDocument()) {
    document->commands.undo();
  }
}

void application::on_redo()
{
  if (auto* document = mModel.GetActiveDocument()) {
    document->commands.redo();
  }
}

void application::on_set_command_capacity(const SetCommandCapacityEvent& event)
{
  mModel.SetCommandCapacity(event.capacity);
}

void application::on_save()
{
  if (auto* document = mModel.GetActiveDocument()) {
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

void application::on_save_as(const SaveAsEvent& event)
{
  if (auto* document = mModel.GetActiveDocument()) {
    document->path = event.path;
    on_save();
  }
}

void application::on_open_save_as_dialog()
{
  if (mModel.HasActiveDocument()) {
    OpenSaveAsDialog(mDispatcher);
  }
}

void application::on_show_settings()
{
  mWidgets.ShowSettings();
}

void application::on_show_new_map_dialog()
{
  mWidgets.ShowNewMapDialog();
}

void application::on_show_open_map_dialog()
{
  mWidgets.ShowOpenMapDialog();
}

void application::on_show_map_properties()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    auto& current = registry->ctx<comp::active_attribute_context>();
    current.entity = entt::null;
  }
}

void application::on_create_map(const CreateMapEvent& event)
{
  const auto id = mModel.AddMap(event.tile_width,
                                event.tile_height,
                                event.row_count,
                                event.column_count);
  mModel.SelectMap(id);
}

void application::on_close_map(const CloseMapEvent& event)
{
  if (mModel.HasPath(event.id)) {
    set_last_closed_file(mModel.GetPath(event.id));
  }
  mModel.RemoveMap(event.id);
}

void application::on_open_map(const OpenMapEvent& event)
{
  /* Just silently ignore the request if the map is already open */
  if (mModel.HasDocumentWithPath(event.path)) {
    log_warning("Tried to open map that was already open!");
    return;
  }

  parsing::map_parser parser;
  parser.parse_map(event.path);

  if (parser.is_okay()) {
    mModel.AddMap(restore_document_from_ir(parser.data(), mTextures));
    add_file_to_history(event.path);
  }
  else {
    mWidgets.ShowMapImportErrorDialog(parser.error());
  }
}

void application::on_select_map(const SelectMapEvent& event)
{
  mModel.SelectMap(event.id);
}

void application::on_select_tool(const SelectToolEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::SelectTool(registry, event.type);
}

void application::on_mouse_pressed(const MousePressedEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    sys::ToolOnPressed(*registry, mDispatcher, event.info);
  }
}

void application::on_mouse_drag(const MouseDragEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    sys::ToolOnDragged(*registry, mDispatcher, event.info);
  }
}

void application::on_mouse_released(const MouseReleasedEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    sys::ToolOnReleased(*registry, mDispatcher, event.info);
  }
}

void application::on_stamp_sequence(StampSequenceEvent event)
{
  _register<StampSequenceCmd>(mModel,
                              std::move(event.old_state),
                              std::move(event.sequence));
}

void application::on_eraser_sequence(EraserSequenceEvent event)
{
  _register<EraserSequenceCmd>(mModel, std::move(event.old_state));
}

void application::on_flood(const FloodEvent& event)
{
  _execute<BucketCmd>(mModel, event.origin, event.replacement);
}

void application::on_center_viewport()
{
  CenterViewport();
}

void application::on_offset_viewport(const OffsetViewportEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::OffsetViewport(registry, event.dx, event.dy);
}

void application::on_offset_bound_viewport(const OffsetBoundViewportEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::OffsetBoundViewport(registry,
                           event.entity,
                           event.dx,
                           event.dy,
                           event.view_width,
                           event.view_height);
}

void application::on_pan_left()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::PanViewportLeft(registry);
}

void application::on_pan_right()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::PanViewportRight(registry);
}

void application::on_pan_up()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::PanViewportUp(registry);
}

void application::on_pan_down()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::PanViewportDown(registry);
}

void application::on_increase_zoom()
{
  auto& registry = mModel.GetActiveRegistryRef();
  const auto mousePos = ImGui::GetIO().MousePos;
  sys::IncreaseViewportZoom(registry, mousePos.x, mousePos.y);
}

void application::on_decrease_zoom()
{
  auto& registry = mModel.GetActiveRegistryRef();
  const auto mousePos = ImGui::GetIO().MousePos;
  sys::DecreaseViewportZoom(registry, mousePos.x, mousePos.y);
}

void application::on_reset_zoom()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::ResetViewportZoom(registry);
}

void application::on_show_add_tileset_dialog()
{
  mWidgets.ShowAddTilesetDialog();
}

void application::on_add_tileset(const AddTilesetEvent& event)
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

void application::on_remove_tileset(const RemoveTilesetEvent& event)
{
  _execute<RemoveTilesetCmd>(mModel, event.id);
}

void application::on_select_tileset(const SelectTilesetEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::select_tileset(registry, event.id);
}

void application::on_set_tileset_selection(const SetTilesetSelectionEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::update_tileset_selection(registry, event.selection);
}

void application::on_set_tileset_name(const SetTilesetNameEvent& event)
{
  _execute<SetTilesetNameCmd>(mModel, event.id, event.name);
}

void application::on_add_row()
{
  _execute<AddRowCmd>(mModel);
}

void application::on_add_column()
{
  _execute<AddColumnCmd>(mModel);
}

void application::on_remove_row()
{
  _execute<RemoveRowCmd>(mModel);
}

void application::on_remove_column()
{
  _execute<RemoveColumnCmd>(mModel);
}

void application::on_resize_map(const ResizeMapEvent& event)
{
  _execute<ResizeMapCmd>(mModel, event.row_count, event.col_count);
}

void application::on_open_resize_map_dialog()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    const auto& map = registry->ctx<MapInfo>();
    mWidgets.ShowResizeMapDialog(map.row_count, map.column_count);
  }
}

void application::on_add_layer(const AddLayerEvent& event)
{
  _execute<AddLayerCmd>(mModel, event.type);
}

void application::on_remove_layer(const RemoveLayerEvent& event)
{
  _execute<RemoveLayerCmd>(mModel, event.id);
}

void application::on_select_layer(const SelectLayerEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    auto& active = registry->ctx<comp::active_layer>();
    active.entity = sys::find_layer(*registry, event.id);
  }
}

void application::on_move_layer_up(const MoveLayerUpEvent& event)
{
  _execute<MoveLayerUpCmd>(mModel, event.id);
}

void application::on_move_layer_down(const MoveLayerDownEvent& event)
{
  _execute<MoveLayerDownCmd>(mModel, event.id);
}

void application::on_duplicate_layer(const DuplicateLayerEvent& event)
{
  _execute<DuplicateLayerCmd>(mModel, event.id);
}

void application::on_set_layer_opacity(const SetLayerOpacityEvent& event)
{
  _execute<SetLayerOpacityCmd>(mModel, event.id, event.opacity);
}

void application::on_set_layer_visible(const SetLayerVisibleEvent& event)
{
  _execute<SetLayerVisibilityCmd>(mModel, event.id, event.visible);
}

void application::on_open_rename_layer_dialog(const OpenRenameLayerDialogEvent& event)
{
  mWidgets.ShowRenameLayerDialog(event.id);
}

void application::on_rename_layer(const RenameLayerEvent& event)
{
  _execute<RenameLayerCmd>(mModel, event.id, event.name);
}

void application::on_set_object_name(const SetObjectNameEvent& event)
{
  _execute<SetObjectNameCmd>(mModel, event.id, event.name);
}

void application::on_move_object(const MoveObjectEvent& event)
{
  _register<MoveObjectCmd>(mModel,
                           event.id,
                           event.old_x,
                           event.old_y,
                           event.new_x,
                           event.new_y);
}

void application::on_set_object_visibility(const SetObjectVisibilityEvent& event)
{
  _execute<SetObjectVisibilityCmd>(mModel, event.id, event.visible);
}

void application::on_set_object_tag(const SetObjectTagEvent& event)
{
  _execute<SetObjectTagCmd>(mModel, event.id, event.tag);
}

void application::on_spawn_object_context_menu(const SpawnObjectContextMenuEvent&)
{
  OpenObjectContextMenu();
}

void application::on_show_add_property_dialog()
{
  mWidgets.ShowAddPropertyDialog();
}

void application::on_show_rename_property_dialog(
    const ShowRenamePropertyDialogEvent& event)
{
  mWidgets.ShowRenamePropertyDialog(event.name);
}

void application::on_show_change_property_type_dialog(
    const ShowChangePropertyTypeDialogEvent& event)
{
  mWidgets.ShowChangePropertyTypeDialog(event.name, event.current_type);
}

void application::on_add_property(const AddPropertyEvent& event)
{
  _execute<AddPropertyCmd>(mModel, event.name, event.type);
}

void application::on_remove_property(const RemovePropertyEvent& event)
{
  _execute<RemovePropertyCmd>(mModel, event.name);
}

void application::on_rename_property(const RenamePropertyEvent& event)
{
  _execute<RenamePropertyCmd>(mModel, event.old_name, event.new_name);
}

void application::on_update_property(const UpdatePropertyEvent& event)
{
  _execute<UpdatePropertyCmd>(mModel, event.name, event.value);
}

void application::on_change_property_type(const ChangePropertyTypeEvent& event)
{
  _execute<ChangePropertyTypeCmd>(mModel, event.name, event.type);
}

void application::on_inspect_context(const InspectContextEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  auto& current = registry.ctx<comp::active_attribute_context>();
  current.entity = event.entity;
}

void application::on_open_component_editor()
{
  mWidgets.ShowComponentEditor(mModel);
}

void application::on_create_component_def(const CreateComponentDefEvent& event)
{
  _execute<CreateComponentDefCmd>(mModel, event.name);
}

void application::on_remove_component_def(const RemoveComponentDefEvent& event)
{
  _execute<RemoveComponentDefCmd>(mModel, event.id);
}

void application::on_rename_component_def(const RenameComponentDefEvent& event)
{
  _execute<RenameComponentCmd>(mModel, event.id, event.name);
}

void application::on_create_component_attribute(
    const CreateComponentAttributeEvent& event)
{
  _execute<CreateComponentAttributeCmd>(mModel, event.id, event.name);
}

void application::on_remove_component_attribute(
    const RemoveComponentAttributeEvent& event)
{
  _execute<RemoveComponentAttributeCmd>(mModel, event.id, event.name);
}

void application::on_rename_component_attribute(
    const RenameComponentAttributeEvent& event)
{
  _execute<RenameComponentAttributeCmd>(mModel, event.id, event.previous, event.updated);
}

void application::on_duplicate_component_attribute(
    const DuplicateComponentAttributeEvent& event)
{
  _execute<DuplicateComponentAttributeCmd>(mModel, event.id, event.attribute);
}

void application::on_set_component_attribute_type(
    const SetComponentAttributeTypeEvent& event)
{
  _execute<SetComponentAttributeTypeCmd>(mModel, event.id, event.attribute, event.type);
}

void application::on_update_component_def_attribute(
    const UpdateComponentDefAttributeEvent& event)
{
  _execute<UpdateComponentAttributeCmd>(mModel, event.id, event.attribute, event.value);
}

void application::on_add_component(const AddComponentEvent& event)
{
  _execute<AddComponentCmd>(mModel, event.context, event.component);
}

void application::on_remove_component(const RemoveComponentEvent& event)
{
  _execute<RemoveComponentCmd>(mModel, event.context, event.component);
}

void application::on_update_component(const UpdateComponentEvent& event)
{
  _execute<UpdateComponentCmd>(mModel,
                               event.context,
                               event.component,
                               event.attribute,
                               event.value);
}

void application::on_reset_component_values(const ResetComponentValuesEvent& event)
{
  _execute<ResetComponentCmd>(mModel, event.context, event.component);
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
    mWidgetShowState.prev_show_toolbar = mWidgets.IsToolbarVisible();
  }

  prefs.set_layer_dock_visible(show);
  prefs.set_tileset_dock_visible(show);
  prefs.set_properties_dock_visible(show);
  prefs.set_component_dock_visible(show);
  prefs.set_log_dock_visible(show);

  mWidgets.SetToolbarVisible(show);

  if (show) {
    prefs.set_layer_dock_visible(mWidgetShowState.prev_show_layer_dock);
    prefs.set_tileset_dock_visible(mWidgetShowState.prev_show_tileset_dock);
    prefs.set_properties_dock_visible(mWidgetShowState.prev_show_properties_dock);
    prefs.set_log_dock_visible(mWidgetShowState.prev_show_log_dock);
    prefs.set_component_dock_visible(mWidgetShowState.prev_show_component_dock);

    mWidgets.SetToolbarVisible(mWidgetShowState.prev_show_toolbar);
  }

  show = !show;
}

void application::on_quit()
{
  mQuit = true;
}

}  // namespace tactile
