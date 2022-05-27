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

#include <boost/stacktrace.hpp>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <glm/vec2.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include "cfg/configuration.hpp"
#include "cfg/fonts.hpp"
#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/components/viewport.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "core/tools/tool_manager.hpp"
#include "core/utils/texture_manager.hpp"
#include "editor/commands/command_stack.hpp"
#include "editor/commands/commands.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/dialogs/save_as_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/layers/layer_dock.hpp"
#include "editor/gui/menus/edit_menu.hpp"
#include "editor/gui/menus/file_menu.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/properties/property_dock.hpp"
#include "editor/gui/tilesets/tileset_dock.hpp"
#include "editor/gui/tilesets/tileset_view.hpp"
#include "editor/gui/viewport/map_view.hpp"
#include "editor/gui/viewport/viewport_widget.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "editor/shortcuts/shortcuts.hpp"
#include "io/maps/parser/parse_map.hpp"
#include "io/maps/restore_map_from_ir.hpp"
#include "io/maps/save_document.hpp"
#include "io/persistence/history.hpp"
#include "io/persistence/preferences.hpp"
#include "io/persistence/session.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

template <typename Command, typename... Args>
[[deprecated]] void _execute(DocumentModel& model, Args&&... args)
{
  if (auto* document = model.active_document()) {
    auto& registry = document->get_registry();
    auto& commands = document->get_history();
    commands.push<Command>(registry, std::forward<Args>(args)...);
  }
  else {
    spdlog::error("Could not execute a command due to no active document!");
  }
}

template <typename Command, typename... Args>
[[deprecated]] void _register(DocumentModel& model, Args&&... args)
{
  if (auto* document = model.active_document()) {
    auto& registry = document->get_registry();
    auto& commands = document->get_history();
    commands.push_without_redo<Command>(registry, std::forward<Args>(args)...);
  }
  else {
    spdlog::error("Could not register a command due to no active document!");
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

  subscribe_to_events();
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

auto Application::active_document() -> ADocument*
{
  return mData->model.active_document();
}

void Application::subscribe_to_events()
{
  using Self = Application;
  auto& d = get_dispatcher();

  // clang-format off
  d.sink<UndoEvent>().connect<&Self::on_undo>(this);
  d.sink<RedoEvent>().connect<&Self::on_redo>(this);
  d.sink<SetCommandCapacityEvent>().connect<&Self::on_set_command_capacity>(this);

  d.sink<SaveEvent>().connect<&Self::on_save>(this);
  d.sink<SaveAsEvent>().connect<&Self::on_save_as>(this);
  d.sink<OpenSaveAsDialogEvent>().connect<&Self::on_open_save_as_dialog>(this);
  d.sink<ShowSettingsEvent>().connect<&show_settings_dialog>();

  d.sink<ShowNewMapDialogEvent>().connect<&show_map_creation_dialog>();
  d.sink<ShowOpenMapDialogEvent>().connect<&show_map_selector_dialog>();
  d.sink<InspectMapEvent>().connect<&Self::on_show_map_properties>(this);
  d.sink<CreateMapEvent>().connect<&Self::on_create_map>(this);
  d.sink<OpenMapEvent>().connect<&Self::on_open_map>(this);

  d.sink<OpenDocumentEvent>().connect<&Self::on_open_document>(this);
  d.sink<CloseDocumentEvent>().connect<&Self::on_close_document>(this);
  d.sink<SelectDocumentEvent>().connect<&Self::on_select_document>(this);

  d.sink<SelectToolEvent>().connect<&Self::on_select_tool>(this);
  d.sink<ToolPressedEvent>().connect<&Self::on_tool_pressed>(this);
  d.sink<ToolDraggedEvent>().connect<&Self::on_tool_dragged>(this);
  d.sink<ToolReleasedEvent>().connect<&Self::on_tool_released>(this);
  d.sink<ToolEnteredEvent>().connect<&Self::on_tool_entered>(this);
  d.sink<ToolExitedEvent>().connect<&Self::on_tool_exited>(this);

  d.sink<StampSequenceEvent>().connect<&Self::on_stamp_sequence>(this);
  d.sink<SetStampRandomizerEvent>().connect<&Self::on_set_stamp_randomizer_event>(this);
  d.sink<EraserSequenceEvent>().connect<&Self::on_eraser_sequence>(this);
  d.sink<FloodEvent>().connect<&Self::on_flood>(this);
  d.sink<AddRectangleEvent>().connect<&Self::on_add_rectangle>(this);
  d.sink<AddEllipseEvent>().connect<&Self::on_add_ellipse>(this);
  d.sink<AddPointEvent>().connect<&Self::on_add_point>(this);

  d.sink<CenterViewportEvent>().connect<&center_map_viewport>();
  d.sink<OffsetViewportEvent>().connect<&Self::on_offset_viewport>(this);
  d.sink<OffsetBoundViewportEvent>().connect<&Self::on_offset_bound_viewport>(this);
  d.sink<PanLeftEvent>().connect<&Self::on_pan_left>(this);
  d.sink<PanRightEvent>().connect<&Self::on_pan_right>(this);
  d.sink<PanUpEvent>().connect<&Self::on_pan_up>(this);
  d.sink<PanDownEvent>().connect<&Self::on_pan_down>(this);
  d.sink<IncreaseZoomEvent>().connect<&Self::on_increase_zoom>(this);
  d.sink<DecreaseZoomEvent>().connect<&Self::on_decrease_zoom>(this);
  d.sink<ResetZoomEvent>().connect<&Self::on_reset_zoom>(this);
  d.sink<ResetFontSizeEvent>().connect<&Self::on_reset_font_size>(this);
  d.sink<IncreaseFontSizeEvent>().connect<&Self::on_increase_font_size>(this);
  d.sink<DecreaseFontSizeEvent>().connect<&Self::on_decrease_font_size>(this);

  d.sink<ShowTilesetCreationDialogEvent>().connect<&show_tileset_creation_dialog>();
  d.sink<AddTilesetEvent>().connect<&Self::on_add_tileset>(this);
  d.sink<RemoveTilesetEvent>().connect<&Self::on_remove_tileset>(this);
  d.sink<SelectTilesetEvent>().connect<&Self::on_select_tileset>(this);
  d.sink<SetTilesetSelectionEvent>().connect<&Self::on_set_tileset_selection>(this);
  d.sink<SetTilesetNameEvent>().connect<&Self::on_set_tileset_name>(this);

  d.sink<AddRowEvent>().connect<&Self::on_add_row>(this);
  d.sink<AddColumnEvent>().connect<&Self::on_add_column>(this);
  d.sink<RemoveRowEvent>().connect<&Self::on_remove_row>(this);
  d.sink<RemoveColumnEvent>().connect<&Self::on_remove_column>(this);
  d.sink<ResizeMapEvent>().connect<&Self::on_resize_map>(this);
  d.sink<OpenResizeMapDialogEvent>().connect<&Self::on_open_resize_map_dialog>(this);

  d.sink<AddLayerEvent>().connect<&Self::on_add_layer>(this);
  d.sink<RemoveLayerEvent>().connect<&Self::on_remove_layer>(this);
  d.sink<SelectLayerEvent>().connect<&Self::on_select_layer>(this);
  d.sink<MoveLayerUpEvent>().connect<&Self::on_move_layer_up>(this);
  d.sink<MoveLayerDownEvent>().connect<&Self::on_move_layer_down>(this);
  d.sink<DuplicateLayerEvent>().connect<&Self::on_duplicate_layer>(this);
  d.sink<SetLayerOpacityEvent>().connect<&Self::on_set_layer_opacity>(this);
  d.sink<SetLayerVisibleEvent>().connect<&Self::on_set_layer_visible>(this);
  d.sink<OpenRenameLayerDialogEvent>().connect<&Self::on_open_rename_layer_dialog>(this);
  d.sink<RenameLayerEvent>().connect<&Self::on_rename_layer>(this);

  d.sink<SetObjectNameEvent>().connect<&Self::on_set_object_name>(this);
  d.sink<MoveObjectEvent>().connect<&Self::on_move_object>(this);
  d.sink<SetObjectVisibilityEvent>().connect<&Self::on_set_object_visibility>(this);
  d.sink<SetObjectTagEvent>().connect<&Self::on_set_object_tag>(this);
  d.sink<SpawnObjectContextMenuEvent>().connect<&Self::on_spawn_object_context_menu>(this);

  d.sink<ShowAddPropertyDialogEvent>().connect<&show_property_creation_dialog>();
  d.sink<ShowRenamePropertyDialogEvent>().connect<&Self::on_show_rename_property_dialog>(this);
  d.sink<ShowChangePropertyTypeDialogEvent>().connect<&Self::on_show_change_property_type_dialog>(this);
  d.sink<AddPropertyEvent>().connect<&Self::on_add_property>(this);
  d.sink<RemovePropertyEvent>().connect<&Self::on_remove_property>(this);
  d.sink<RenamePropertyEvent>().connect<&Self::on_rename_property>(this);
  d.sink<UpdatePropertyEvent>().connect<&Self::on_update_property>(this);
  d.sink<ChangePropertyTypeEvent>().connect<&Self::on_change_property_type>(this);
  d.sink<InspectContextEvent>().connect<&Self::on_inspect_context>(this);

  d.sink<OpenComponentEditorEvent>().connect<&Self::on_open_component_editor>(this);
  d.sink<CreateComponentDefEvent>().connect<&Self::on_create_component_def>(this);
  d.sink<RemoveComponentDefEvent>().connect<&Self::on_remove_component_def>(this);
  d.sink<RenameComponentDefEvent>().connect<&Self::on_rename_component_def>(this);
  d.sink<CreateComponentAttrEvent>().connect<&Self::on_create_component_attr>(this);
  d.sink<RemoveComponentAttrEvent>().connect<&Self::on_remove_component_attr>(this);
  d.sink<RenameComponentAttrEvent>().connect<&Self::on_rename_component_attr>(this);
  d.sink<DuplicateComponentAttrEvent>().connect<&Self::on_duplicate_component_attr>(this);
  d.sink<SetComponentAttrTypeEvent>().connect<&Self::on_set_component_attr_type>(this);
  d.sink<UpdateComponentDefAttrEvent>().connect<&Self::on_update_component_def_attr>(this);

  d.sink<AddComponentEvent>().connect<&Self::on_add_component>(this);
  d.sink<RemoveComponentEvent>().connect<&Self::on_remove_component>(this);
  d.sink<UpdateComponentEvent>().connect<&Self::on_update_component>(this);
  d.sink<ResetComponentValuesEvent>().connect<&Self::on_reset_component_values>(this);

  d.sink<ToggleUiEvent>().connect<&Self::on_toggle_ui>(this);
  d.sink<ReloadFontsEvent>().connect<&Self::on_reload_fonts>(this);
  d.sink<QuitEvent>().connect<&Self::on_quit>(this);
  // clang-format on
}

auto Application::get_dispatcher() -> entt::dispatcher&
{
  return mData->dispatcher;
}

void Application::save_current_files_to_history()
{
  mData->model.each([this](const UUID& id) {
    // TODO
    const auto document = mData->model.get_document(id);
    // if (document.open && document.has_path()) {
    //   add_file_to_history(document.path());
    // }
  });
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
  /* This function is ugly and awkward, but there doesn't seem to be a good way to handle
     mouse "wheel" events using the public ImGui APIs. Otherwise, it would be nicer to
     keep this code closer to the actual widgets. */

  constexpr float scaling = 4.0f;
  const auto* document = active_document();

  if (document && !ImGui::GetTopMostPopupModal()) {
    const auto& registry = document->get_registry();
    if (is_mouse_within_viewport()) {
      const auto& viewport = ctx_get<comp::Viewport>(registry);
      if (cen::is_active(primary_modifier)) {
        const auto y = event.precise_y();
        if (y > 0) {
          mData->dispatcher.enqueue<IncreaseZoomEvent>();
        }
        else if (y < 0 && sys::can_decrease_viewport_zoom(registry)) {
          mData->dispatcher.enqueue<DecreaseZoomEvent>();
        }
      }
      else {
        const auto dx = event.precise_x() * (viewport.tile_width / scaling);
        const auto dy = event.precise_y() * (viewport.tile_height / scaling);
        mData->dispatcher.enqueue<OffsetViewportEvent>(-dx, dy);
      }
    }
    else if (is_tileset_dock_hovered()) {
      const auto width = get_tileset_view_width();
      const auto height = get_tileset_view_height();
      if (width && height) {
        const auto entity = sys::find_active_tileset(registry);
        TACTILE_ASSERT(entity != entt::null);

        const auto& viewport = checked_get<comp::Viewport>(registry, entity);

        const auto dx = event.precise_x() * (viewport.tile_width / scaling);
        const auto dy = event.precise_y() * (viewport.tile_height / scaling);
        mData->dispatcher.enqueue<OffsetBoundViewportEvent>(entity,
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
  if (auto* document = active_document()) {
    auto& commands = document->get_history();
    commands.undo();
  }
}

void Application::on_redo()
{
  if (auto* document = active_document()) {
    auto& commands = document->get_history();
    commands.redo();
  }
}

void Application::on_set_command_capacity(const SetCommandCapacityEvent& event)
{
  mData->model.set_command_capacity(event.capacity);
}

void Application::on_save()
{
  if (auto* document = active_document()) {
    if (document->has_path()) {
      if (document->is_map()) {
        save_document(mData->model, document->id());

        auto& commands = document->get_history();
        commands.mark_as_clean();

        // auto& context = ctx_get<comp::AttributeContext>(document->registry);
        // context.name = document->path().filename().string();
      }
      else {
        spdlog::warn("Cannot yet save changes to tilesets!");
      }
    }
    else {
      on_open_save_as_dialog();
    }
  }
}

void Application::on_save_as(const SaveAsEvent& event)
{
  if (auto* document = active_document()) {
    document->set_path(event.path);
    on_save();
  }
}

void Application::on_open_save_as_dialog()
{
  if (active_document() != nullptr) {
    show_save_as_dialog(mData->dispatcher);
  }
}

void Application::on_show_map_properties()
{
  if (auto* document = active_document()) {
    auto& registry = document->get_registry();
    auto& current = ctx_get<comp::ActiveAttributeContext>(registry);
    current.entity = entt::null;
  }
}

void Application::on_create_map(const CreateMapEvent& event)
{
  const auto id = mData->model.add_map({event.tile_width, event.tile_height},
                                       event.row_count,
                                       event.column_count);
  mData->model.select_document(id);
}

void Application::on_close_document(const CloseDocumentEvent& event)
{
  const auto document = mData->model.get_document(event.id);

  if (document->is_map() && document->has_path()) {
    set_last_closed_file(document->get_path());
  }

  mData->model.close_document(event.id);
}

void Application::on_open_document(const OpenDocumentEvent& event)
{
  mData->model.open_document(event.document_id);
}

// TODO consider renaming event
void Application::on_open_map(const OpenMapEvent& event)
{
  /* Just silently ignore the request if the map is already open */
  if (mData->model.has_document_with_path(event.path)) {
    spdlog::warn("Tried to open map that was already open!");
    return;
  }

  const auto ir = parsing::parse_map(event.path);
  if (ir.error() == parsing::ParseError::None) {
    restore_map_from_ir(ir, mData->model, mData->textures);
    add_file_to_history(event.path);
  }
  else {
    mData->widgets.show_map_import_error_dialog(ir.error());
  }
}

void Application::on_select_document(const SelectDocumentEvent& event)
{
  mData->model.select_document(event.id);
}

void Application::on_select_tool(const SelectToolEvent& event)
{
  if (auto* document = active_document()) {
    if (document->is_map()) {
      auto& registry = document->get_registry();
      auto& tools = ctx_get<ToolManager>(registry);
      tools.select_tool(event.type, registry, mData->dispatcher);
    }
  }
}

void Application::on_tool_pressed(const ToolPressedEvent& event)
{
  if (auto* document = active_document()) {
    if (document->is_map()) {
      auto& registry = document->get_registry();
      auto& tools = ctx_get<ToolManager>(registry);
      tools.on_pressed(registry, mData->dispatcher, event.info);
    }
  }
}

void Application::on_tool_dragged(const ToolDraggedEvent& event)
{
  if (auto* document = active_document()) {
    if (document->is_map()) {
      auto& registry = document->get_registry();
      auto& tools = ctx_get<ToolManager>(registry);
      tools.on_dragged(registry, mData->dispatcher, event.info);
    }
  }
}

void Application::on_tool_released(const ToolReleasedEvent& event)
{
  if (auto* document = active_document()) {
    if (document->is_map()) {
      auto& registry = document->get_registry();
      auto& tools = ctx_get<ToolManager>(registry);
      tools.on_released(registry, mData->dispatcher, event.info);
    }
  }
}

void Application::on_tool_entered()
{
  if (auto* document = active_document()) {
    if (document->is_map()) {
      auto& registry = document->get_registry();
      auto& tools = ctx_get<ToolManager>(registry);
      tools.on_entered(registry, mData->dispatcher);
    }
  }
}

void Application::on_tool_exited()
{
  if (auto* document = active_document()) {
    if (document->is_map()) {
      auto& registry = document->get_registry();
      auto& tools = ctx_get<ToolManager>(registry);
      tools.on_exited(registry, mData->dispatcher);
    }
  }
}

void Application::on_stamp_sequence(StampSequenceEvent event)
{
  _register<StampToolCmd>(mData->model,
                          std::move(event.old_state),
                          std::move(event.sequence));
}

void Application::on_set_stamp_randomizer_event(const SetStampRandomizerEvent& event)
{
  auto& registry = mData->model.get_active_registry();
  auto& tools = ctx_get<ToolManager>(registry);
  tools.set_stamp_random_mode(event.enabled);
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

void Application::on_offset_viewport(const OffsetViewportEvent& event)
{
  auto& registry = mData->model.get_active_registry();
  sys::offset_viewport(registry, event.dx, event.dy);
}

void Application::on_offset_bound_viewport(const OffsetBoundViewportEvent& event)
{
  // FIXME broken
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

void Application::on_add_tileset(const AddTilesetEvent& event)
{
  if (auto info = mData->textures.load(event.path)) {
    mData->model.add_tileset(*info, {event.tile_width, event.tile_height});
  }
  else {
    spdlog::error("Failed to load tileset texture!");
  }
}

void Application::on_remove_tileset(const RemoveTilesetEvent& event)
{
  mData->model.remove_tileset(event.tileset_id);
}

void Application::on_select_tileset(const SelectTilesetEvent& event)
{
  auto& registry = mData->model.get_active_registry();
  sys::select_tileset(registry, event.tileset_id);
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
    const auto& map = ctx_get<comp::MapInfo>(*registry);
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
    sys::select_layer(*registry, event.id);
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
  show_rename_layer_dialog(event.id);
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

void Application::on_show_rename_property_dialog(
    const ShowRenamePropertyDialogEvent& event)
{
  show_rename_property_dialog(event.name);
}

void Application::on_show_change_property_type_dialog(
    const ShowChangePropertyTypeDialogEvent& event)
{
  show_change_property_type_dialog(event.name, event.current_type);
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
  auto& current = ctx_get<comp::ActiveAttributeContext>(registry);
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
