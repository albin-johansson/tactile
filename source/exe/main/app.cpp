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

#include "app.hpp"

#include <utility>  // move

#include <centurion/system.hpp>
#include <fmt/chrono.h>
#include <fmt/std.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include "common/fmt/attribute_formatter.hpp"
#include "common/fmt/attribute_type_formatter.hpp"
#include "common/fmt/entity_formatter.hpp"
#include "common/fmt/lang_formatter.hpp"
#include "common/fmt/layer_type_formatter.hpp"
#include "common/fmt/menu_action_formatter.hpp"
#include "common/fmt/overlay_pos_formatter.hpp"
#include "common/fmt/theme_formatter.hpp"
#include "common/fmt/tile_compression_formatter.hpp"
#include "common/fmt/tile_encoding_formatter.hpp"
#include "common/fmt/vector_formatter.hpp"
#include "io/directories.hpp"
#include "io/file_history_io.hpp"
#include "io/session_io.hpp"
#include "io/settings_io.hpp"
#include "model/events/delegates/command_delegate.hpp"
#include "model/events/delegates/component_delegate.hpp"
#include "model/events/delegates/file_delegate.hpp"
#include "model/events/delegates/font_delegate.hpp"
#include "model/events/delegates/input_delegate.hpp"
#include "model/events/delegates/layer_delegate.hpp"
#include "model/events/delegates/map_delegate.hpp"
#include "model/events/delegates/menu_delegate.hpp"
#include "model/events/delegates/object_delegate.hpp"
#include "model/events/delegates/property_delegate.hpp"
#include "model/events/delegates/settings_delegate.hpp"
#include "model/events/delegates/tileset_delegate.hpp"
#include "model/events/delegates/tool_delegate.hpp"
#include "model/events/delegates/viewport_delegate.hpp"
#include "model/i18n/language_system.hpp"
#include "model/model_factory.hpp"
#include "model/persistence/file_history_components.hpp"
#include "model/persistence/file_history_system.hpp"
#include "model/persistence/settings.hpp"
#include "model/textures/texture_components.hpp"
#include "model/textures/texture_factory.hpp"
#include "model/textures/texture_system.hpp"
#include "model/view/menu_system.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/style/fonts.hpp"
#include "ui/style/themes.hpp"
#include "ui/ui.hpp"
#include "ui/widget_state.hpp"

// Shorthand used when connecting events to slot functions
#define ROUTE(Event, Slot) mDispatcher.sink<Event>().connect<Slot>(this)

namespace tactile {

void App::on_startup(const BackendAPI api)
{
  mModel = std::make_unique<Model>();
  auto& model = *mModel;

  sys::init_model(model, api);

  _subscribe_to_events();

  auto& icons = model.get<Icons>();
  icons.tactile_icon = sys::create_texture(model, find_resource("assets/icon.png"));

  _init_persistent_settings();

  sys::init_menus(model);
}

void App::_subscribe_to_events()
{
  // Command events
  ROUTE(UndoEvent, &App::_on_undo);
  ROUTE(RedoEvent, &App::_on_redo);
  ROUTE(SetCommandCapacityEvent, &App::_on_set_command_capacity);

  // File events
  ROUTE(OpenDocumentEvent, &App::_on_open_document);
  ROUTE(CloseDocumentEvent, &App::_on_close_document);
  ROUTE(SelectDocumentEvent, &App::_on_select_document);
  ROUTE(SaveEvent, &App::_on_save);
  ROUTE(SaveAsEvent, &App::_on_save_as);
  ROUTE(ShowSaveAsDialogEvent, &App::_on_show_save_as_dialog);
  ROUTE(ReopenLastClosedFileEvent, &App::_on_reopen_last_closed_file);
  ROUTE(ClearFileHistoryEvent, &App::_on_clear_file_history);
  ROUTE(QuitEvent, &App::_on_quit);

  // View events
  ROUTE(ResetLayoutEvent, &App::_on_reset_layout);
  ROUTE(ToggleHighlightLayerEvent, &App::_on_toggle_highlight_layer);

  // Map events
  ROUTE(ShowNewMapDialogEvent, &App::_on_show_new_map_dialog);
  ROUTE(ShowOpenMapDialogEvent, &App::_on_show_open_map_dialog);
  ROUTE(ShowResizeMapDialogEvent, &App::_on_show_resize_map_dialog);
  ROUTE(ShowGodotExportDialogEvent, &App::_on_show_godot_export_dialog);
  ROUTE(CreateMapEvent, &App::_on_create_map);
  ROUTE(OpenMapEvent, &App::_on_open_map);
  ROUTE(ResizeMapEvent, &App::_on_resize_map);
  ROUTE(AddRowEvent, &App::_on_add_row);
  ROUTE(AddColumnEvent, &App::_on_add_column);
  ROUTE(RemoveRowEvent, &App::_on_remove_row);
  ROUTE(RemoveColumnEvent, &App::_on_remove_column);
  ROUTE(FixTilesInMapEvent, &App::_on_fix_tiles_in_map);
  ROUTE(ExportAsGodotSceneEvent, &App::_on_export_as_godot_scene);
  ROUTE(InspectMapEvent, &App::_on_inspect_map);
  ROUTE(SetTileFormatEncodingEvent, &App::_on_set_tile_format_encoding);
  ROUTE(SetTileFormatCompressionEvent, &App::_on_set_tile_format_compression);
  ROUTE(SetZlibCompressionLevelEvent, &App::_on_set_zlib_compression_level);
  ROUTE(SetZstdCompressionLevelEvent, &App::_on_set_zstd_compression_level);

  // Tileset events
  ROUTE(ShowNewTilesetDialogEvent, &App::_on_show_new_tileset_dialog);
  ROUTE(CreateTilesetEvent, &App::_on_create_tileset);
  ROUTE(DetachTilesetEvent, &App::_on_detach_tileset);
  ROUTE(SelectTilesetEvent, &App::_on_select_tileset);
  ROUTE(SetTilesetSelectionEvent, &App::_on_set_tileset_selection);
  ROUTE(RenameTilesetEvent, &App::_on_rename_tileset);
  ROUTE(SelectTilesetTileEvent, &App::_on_select_tileset_tile);

  // Tile events
  ROUTE(AddAnimationFrameEvent, &App::_on_add_animation_frame);
  ROUTE(SetAnimationFrameDurationEvent, &App::_on_set_animation_frame_duration);
  ROUTE(EnableAnimationFrameSelectionMode,
        &App::_on_enable_animation_frame_selection_mode);
  ROUTE(RemoveAnimationFrameEvent, &App::_on_remove_animation_frame);
  ROUTE(MoveAnimationFrameForwardsEvent, &App::_on_move_animation_frame_forwards);
  ROUTE(MoveAnimationFrameBackwardsEvent, &App::_on_move_animation_frame_backwards);
  ROUTE(RenameTileEvent, &App::_on_rename_tile);

  // Layer events
  ROUTE(CreateLayerEvent, &App::_on_create_layer);
  ROUTE(RemoveLayerEvent, &App::_on_remove_layer);
  ROUTE(RenameLayerEvent, &App::_on_rename_layer);
  ROUTE(DuplicateLayerEvent, &App::_on_duplicate_layer);
  ROUTE(SelectLayerEvent, &App::_on_select_layer);
  ROUTE(MoveLayerUpEvent, &App::_on_move_layer_up);
  ROUTE(MoveLayerDownEvent, &App::_on_move_layer_down);
  ROUTE(SetLayerOpacityEvent, &App::_on_set_layer_opacity);
  ROUTE(SetLayerVisibleEvent, &App::_on_set_layer_visible);
  ROUTE(ShowRenameLayerDialogEvent, &App::_on_show_layer_rename_dialog);

  // Object events
  ROUTE(MoveObjectEvent, &App::_on_move_object);
  ROUTE(MoveObjectEvent, &App::_on_move_object);
  ROUTE(SetObjectVisibleEvent, &App::_on_set_object_visible);
  ROUTE(SetObjectTagEvent, &App::_on_set_object_tag);
  ROUTE(SetObjectNameEvent, &App::_on_set_object_name);
  ROUTE(DuplicateObjectEvent, &App::_on_duplicate_object);
  ROUTE(RemoveObjectEvent, &App::_on_remove_object);
  ROUTE(SpawnObjectContextMenuEvent, &App::_on_spawn_object_context_menu);

  // Tool events
  ROUTE(StampSequenceEvent, &App::_on_stamp_sequence);
  ROUTE(FloodEvent, &App::_on_flood);

  // Font events
  ROUTE(ReloadFontsEvent, &App::_on_reload_fonts);
  ROUTE(IncreaseFontSizeEvent, &App::_on_increase_font_size);
  ROUTE(DecreaseFontSizeEvent, &App::_on_decrease_font_size);
  ROUTE(ResetFontSizeEvent, &App::_on_reset_font_size);

  // Setting events
  ROUTE(ShowSettingsEvent, &App::_on_show_settings);
  ROUTE(SetSettingsEvent, &App::_on_set_settings);
  ROUTE(SetFlagSettingEvent, &App::_on_set_flag_setting);
  ROUTE(NegateFlagSettingEvent, &App::_on_negate_flag_setting);
  ROUTE(SetViewportOverlayPosEvent, &App::_on_set_viewport_overlay_pos);
  ROUTE(SetLanguageEvent, &App::_on_set_language);
  ROUTE(SetThemeEvent, &App::_on_set_theme);
  ROUTE(ResetDockVisibilitiesEvent, &App::_on_reset_dock_visibilities);
  ROUTE(MenuActionEvent, &App::_on_menu_action);

  // Viewport events
  ROUTE(ViewportMousePressedEvent, &App::_on_viewport_mouse_pressed);
  ROUTE(ViewportMouseDraggedEvent, &App::_on_viewport_mouse_dragged);
  ROUTE(ViewportMouseReleasedEvent, &App::_on_viewport_mouse_released);
  ROUTE(ViewportMouseEnteredEvent, &App::_on_viewport_mouse_entered);
  ROUTE(ViewportMouseExitedEvent, &App::_on_viewport_mouse_exited);
  ROUTE(CenterViewportEvent, &App::_on_center_viewport);
  ROUTE(ResetViewportZoomEvent, &App::_on_reset_viewport_zoom);
  ROUTE(IncreaseViewportZoomEvent, &App::_on_increase_viewport_zoom);
  ROUTE(DecreaseViewportZoomEvent, &App::_on_decrease_viewport_zoom);
  ROUTE(OffsetViewportEvent, &App::_on_offset_viewport);
  ROUTE(SetViewportLimitsEvent, &App::_on_set_viewport_limits);
  ROUTE(SetDynamicViewportInfoEvent, &App::_on_set_dynamic_viewport_info);
  ROUTE(PanViewportUpEvent, &App::_on_pan_viewport_up);
  ROUTE(PanViewportDownEvent, &App::_on_pan_viewport_down);
  ROUTE(PanViewportLeftEvent, &App::_on_pan_viewport_left);
  ROUTE(PanViewportRightEvent, &App::_on_pan_viewport_right);

  // Component events
  ROUTE(ShowComponentEditorEvent, &App::_on_show_component_editor);
  ROUTE(ShowNewCompDialogEvent, &App::_on_show_new_comp_dialog);
  ROUTE(ShowRenameCompDialogEvent, &App::_on_show_rename_comp_dialog);
  ROUTE(ShowNewCompAttrDialogEvent, &App::_on_show_new_comp_attr_dialog);
  ROUTE(ShowRenameCompAttrDialogEvent, &App::_on_show_rename_comp_attr_dialog);
  ROUTE(DefineComponentEvent, &App::_on_define_component);
  ROUTE(UndefComponentEvent, &App::_on_undef_component);
  ROUTE(RenameComponentEvent, &App::_on_rename_component);
  ROUTE(UpdateComponentEvent, &App::_on_update_component);
  ROUTE(AddComponentAttrEvent, &App::_on_add_component_attr);
  ROUTE(RemoveComponentAttrEvent, &App::_on_remove_component_attr);
  ROUTE(RenameComponentAttrEvent, &App::_on_rename_component_attr);
  ROUTE(DuplicateComponentAttrEvent, &App::_on_duplicate_component_attr);
  ROUTE(SetComponentAttrTypeEvent, &App::_on_set_component_attr_type);
  ROUTE(AttachComponentEvent, &App::_on_attach_component);
  ROUTE(DetachComponentEvent, &App::_on_detach_component);
  ROUTE(ResetAttachedComponentEvent, &App::_on_reset_attached_component);
  ROUTE(UpdateAttachedComponentEvent, &App::_on_update_attached_component);

  // Property events
  ROUTE(ShowNewPropertyDialogEvent, &App::_on_show_new_property_dialog);
  ROUTE(ShowRenamePropertyDialogEvent, &App::_on_show_rename_property_dialog);
  ROUTE(ShowSetPropertyTypeDialogEvent, &App::_on_show_set_property_type_dialog);
  ROUTE(InspectContextEvent, &App::_on_inspect_context);
  ROUTE(CreatePropertyEvent, &App::_on_create_property);
  ROUTE(RemovePropertyEvent, &App::_on_remove_property);
  ROUTE(RenamePropertyEvent, &App::_on_rename_property);
  ROUTE(UpdatePropertyEvent, &App::_on_update_property);
  ROUTE(SetPropertyTypeEvent, &App::_on_set_property_type);

  // Miscellaneous events
  ROUTE(ShowAboutDialogEvent, &App::_on_show_about_dialog);
  ROUTE(ShowCreditsDialogEvent, &App::_on_show_credits_dialog);
  ROUTE(ShowAboutImGuiDialogEvent, &App::_on_show_about_imgui_dialog);
  ROUTE(OpenDirectoryInFinderEvent, &App::_on_open_directory_in_finder);
  ROUTE(OpenUrlEvent, &App::_on_open_url);
}

void App::_init_persistent_settings()
{
  auto& model = *mModel;

  auto& settings = model.get<Settings>();
  settings.copy_from(load_settings_from_disk());
  settings.print();

  sys::load_languages(model);

  auto& file_history = model.get<FileHistory>();
  if (auto history = load_file_history_from_disk()) {
    file_history = std::move(*history);
  }

  if (settings.test_flag(SETTINGS_RESTORE_LAST_SESSION_BIT)) {
    load_session_from_disk(model);
  }

  auto& style = ImGui::GetStyle();
  style.WindowBorderSize = settings.test_flag(SETTINGS_WINDOW_BORDER_BIT) ? 1.0f : 0.0f;
  ui::apply_theme(style, settings.get_theme(), settings.get_theme_saturation());
}

void App::on_shutdown()
{
  auto& model = *mModel;

  sys::store_open_documents_in_file_history(model);
  save_settings_to_disk(model.get<Settings>());
  save_session_to_disk(model);

  const auto& file_history = model.get<FileHistory>();
  save_file_history_to_disk(file_history);
}

void App::on_update()
{
  auto& model = *mModel;

  // TODO update animated tiles
  sys::update_menu_items(model, mDispatcher);

  auto& widgets = model.get<ui::WidgetState>();
  ui::render_ui(model, widgets, mDispatcher);
  ui::check_for_missing_layout_file(model,
                                    widgets.dock_space.root_dock_id.value(),
                                    mDispatcher);

  const auto& io = ImGui::GetIO();
  if (mFramebufferScale.x != io.DisplayFramebufferScale.x) {
    mFramebufferScale = io.DisplayFramebufferScale;
    mWantFontReload = true;
  }

  mDispatcher.update();
}

void App::on_event(const cen::event_handler& event)
{
  tactile::on_event(*mModel, mDispatcher, event);
}

void App::reload_font_files()
{
  ui::reload_imgui_fonts(mModel->get<Settings>());
  mWantFontReload = false;
}

void App::_on_menu_action(const MenuActionEvent& event)
{
  spdlog::trace("[MenuActionEvent] action: {}", event.action);
  on_menu_action(*mModel, mDispatcher, event);
}

void App::_on_undo(const UndoEvent& event)
{
  spdlog::trace("[UndoEvent]");
  on_undo(*mModel, event);
}

void App::_on_redo(const RedoEvent& event)
{
  spdlog::trace("[RedoEvent]");
  on_redo(*mModel, event);
}

void App::_on_set_command_capacity(const SetCommandCapacityEvent& event)
{
  spdlog::trace("[SetCommandCapacityEvent] capacity: {}", event.capacity);
  on_set_command_capacity(*mModel, event);
}

void App::_on_open_document(const OpenDocumentEvent& event)
{
  spdlog::trace("[OpenDocumentEvent] document: {}", event.document);
  on_open_document(*mModel, event);
}

void App::_on_close_document(const CloseDocumentEvent& event)
{
  spdlog::trace("[CloseDocumentEvent] document: {}", event.document);
  on_close_document(*mModel, event);
}

void App::_on_select_document(const SelectDocumentEvent& event)
{
  spdlog::trace("[SelectDocumentEvent] document: {}", event.document);
  on_select_document(*mModel, event);
}

void App::_on_save(const SaveEvent& event)
{
  spdlog::trace("[SaveEvent]");
  on_save(*mModel, mDispatcher, event);
}

void App::_on_save_as(const SaveAsEvent& event)
{
  spdlog::trace("[SaveAsEvent]");
  on_save_as(*mModel, mDispatcher, event);
}

void App::_on_show_save_as_dialog(const ShowSaveAsDialogEvent& event)
{
  spdlog::trace("[ShowSaveAsDialogEvent]");
  on_show_save_as_dialog(*mModel, mDispatcher, event);
}

void App::_on_reopen_last_closed_file(const ReopenLastClosedFileEvent& event)
{
  spdlog::trace("[ReopenLastClosedFileEvent]");
  on_reopen_last_closed_file(*mModel, mDispatcher, event);
}

void App::_on_clear_file_history(const ClearFileHistoryEvent& event)
{
  spdlog::trace("[ClearFileHistoryEvent]");
  on_clear_file_history(*mModel, event);
}

void App::_on_quit(const QuitEvent&)
{
  spdlog::trace("[QuitEvent]");
  mShouldStop = true;
}

void App::_on_reset_layout(const ResetLayoutEvent&)
{
  spdlog::trace("[ResetLayoutEvent]");

  const auto& widgets = mModel->get<ui::WidgetState>();
  ui::reset_layout(*mModel, widgets.dock_space.root_dock_id.value(), mDispatcher);
}

void App::_on_toggle_highlight_layer(const ToggleHighlightLayerEvent&)
{
  spdlog::trace("[ToggleHighlightLayerEvent]");

  auto& settings = mModel->get<Settings>();
  settings.negate_flag(SETTINGS_HIGHLIGHT_ACTIVE_LAYER_BIT);
}

void App::_on_show_new_map_dialog(const ShowNewMapDialogEvent& event)
{
  spdlog::trace("[ShowNewMapDialogEvent]");
  on_show_new_map_dialog(*mModel, event);
}

void App::_on_show_open_map_dialog(const ShowOpenMapDialogEvent& event)
{
  spdlog::trace("[ShowOpenMapDialogEvent]");
  on_show_open_map_dialog(*mModel, event);
}

void App::_on_show_resize_map_dialog(const ShowResizeMapDialogEvent& event)
{
  spdlog::trace("[ShowResizeMapDialogEvent]");
  on_show_resize_map_dialog(*mModel, event);
}

void App::_on_show_godot_export_dialog(const ShowGodotExportDialogEvent& event)
{
  spdlog::trace("[ShowGodotExportDialogEvent]");
  on_show_godot_export_dialog(*mModel, event);
}

void App::_on_create_map(const CreateMapEvent& event)
{
  spdlog::trace("[CreateMapEvent] rows: {}, columns: {}, tile size: {}",
                event.row_count,
                event.column_count,
                event.tile_size);
  on_create_map(*mModel, event);
}

void App::_on_open_map(const OpenMapEvent& event)
{
  spdlog::trace("[OpenMapEvent] path: {}", event.path);
  on_open_map(*mModel, event);
}

void App::_on_resize_map(const ResizeMapEvent& event)
{
  spdlog::trace("[ResizeMapEvent] rows: {}, cols: {}", event.row_count, event.col_count);
  on_resize_map(*mModel, event);
}

void App::_on_add_row(const AddRowEvent& event)
{
  spdlog::trace("[AddRowEvent]");
  on_add_row(*mModel, event);
}

void App::_on_add_column(const AddColumnEvent& event)
{
  spdlog::trace("[AddColumnEvent]");
  on_add_column(*mModel, event);
}

void App::_on_remove_row(const RemoveRowEvent& event)
{
  spdlog::trace("[RemoveRowEvent]");
  on_remove_row(*mModel, event);
}

void App::_on_remove_column(const RemoveColumnEvent& event)
{
  spdlog::trace("[RemoveColumnEvent]");
  on_remove_column(*mModel, event);
}

void App::_on_fix_tiles_in_map(const FixTilesInMapEvent& event)
{
  spdlog::trace("[FixTilesInMapEvent]");
  on_fix_tiles_in_map(*mModel, event);
}

void App::_on_export_as_godot_scene(const ExportAsGodotSceneEvent& event)
{
  spdlog::trace("[ExportAsGodotSceneEvent] root: {}", event.root_dir);
  on_export_as_godot_scene(*mModel, event);
}

void App::_on_inspect_map(const InspectMapEvent& event)
{
  spdlog::trace("[InspectMapEvent]");
  on_inspect_map(*mModel, event);
}

void App::_on_set_tile_format_encoding(const SetTileFormatEncodingEvent& event)
{
  spdlog::trace("[SetTileFormatEncodingEvent] encoding: {}", event.encoding);
  on_set_tile_format_encoding(*mModel, event);
}

void App::_on_set_tile_format_compression(const SetTileFormatCompressionEvent& event)
{
  spdlog::trace("[SetTileFormatCompressionEvent] compression: {}", event.compression);
  on_set_tile_format_compression(*mModel, event);
}

void App::_on_set_zlib_compression_level(const SetZlibCompressionLevelEvent& event)
{
  spdlog::trace("[SetZlibCompressionLevelEvent] level: {}", event.level);
  on_set_zlib_compression_level(*mModel, event);
}

void App::_on_set_zstd_compression_level(const SetZstdCompressionLevelEvent& event)
{
  spdlog::trace("[SetZstdCompressionLevelEvent] level: {}", event.level);
  on_set_zstd_compression_level(*mModel, event);
}

void App::_on_show_new_tileset_dialog(const ShowNewTilesetDialogEvent& event)
{
  spdlog::trace("[ShowNewTilesetDialogEvent]");
  on_show_new_tileset_dialog(*mModel, event);
}

void App::_on_create_tileset(const CreateTilesetEvent& event)
{
  spdlog::trace("[CreateTilesetEvent] map: {}, image: {}, tile size: {}",
                event.map,
                event.image_path,
                event.tile_size);
  on_create_tileset(*mModel, event);
}

void App::_on_detach_tileset(const DetachTilesetEvent& event)
{
  spdlog::trace("[DetachTilesetEvent] map: {}, attached tileset: {}",
                event.map,
                event.attached_tileset);
  on_detach_tileset(*mModel, event);
}

void App::_on_select_tileset(const SelectTilesetEvent& event)
{
  spdlog::trace("[SelectTilesetEvent] attached tileset: {}", event.attached_tileset);
  on_select_tileset(*mModel, event);
}

void App::_on_set_tileset_selection(const SetTilesetSelectionEvent& event)
{
  spdlog::trace("[SetTilesetSelectionEvent] attached tileset: {}",  // TODO region
                event.attached_tileset);
  on_set_tileset_selection(*mModel, event);
}

void App::_on_rename_tileset(const RenameTilesetEvent& event)
{
  spdlog::trace("[RenameTilesetEvent] attached tileset: {}, name: {}",
                event.attached_tileset,
                event.name);
  on_rename_tileset(*mModel, event);
}

void App::_on_select_tileset_tile(const SelectTilesetTileEvent& event)
{
  spdlog::trace("[SelectTilesetTileEvent] tile index: {}", event.tile_index);
  on_select_tileset_tile(*mModel, event);
}

void App::_on_add_animation_frame(const AddAnimationFrameEvent& event)
{
  spdlog::trace("[AddAnimationFrameEvent] tile: {}, frame tile index: {}, duration: {}",
                event.tile,
                event.frame_tile_index,
                event.frame_duration);
  on_add_animation_frame(*mModel, event);
}

void App::_on_set_animation_frame_duration(const SetAnimationFrameDurationEvent& event)
{
  spdlog::trace("[SetAnimationFrameDurationEvent] tile: {}, frame: {}, duration: {}",
                event.tile,
                event.frame_index,
                event.duration);
  on_set_animation_frame_duration(*mModel, event);
}

void App::_on_enable_animation_frame_selection_mode(
    const EnableAnimationFrameSelectionMode& event)
{
  spdlog::trace("[EnableAnimationFrameSelectionMode]");
  on_enable_animation_frame_selection_mode(*mModel, event);
}

void App::_on_remove_animation_frame(const RemoveAnimationFrameEvent& event)
{
  spdlog::trace("[RemoveAnimationFrameEvent] tile: {}, frame: {}",
                event.tile,
                event.frame_index);
  on_remove_animation_frame(*mModel, event);
}

void App::_on_move_animation_frame_forwards(const MoveAnimationFrameForwardsEvent& event)
{
  spdlog::trace("[MoveAnimationFrameForwardsEvent] tile: {}", event.tile);
  on_move_animation_frame_forwards(*mModel, event);
}

void App::_on_move_animation_frame_backwards(
    const MoveAnimationFrameBackwardsEvent& event)
{
  spdlog::trace("[MoveAnimationFrameBackwardsEvent] tile: {}", event.tile);
  on_move_animation_frame_backwards(*mModel, event);
}

void App::_on_rename_tile(const RenameTileEvent& event)
{
  spdlog::trace("[RenameTileEvent] tile: {}, name: {}", event.tile, event.name);
  on_rename_tile(*mModel, event);
}

void App::_on_delete_tile_animation(const DeleteTileAnimationEvent& event)
{
  spdlog::trace("[DeleteTileAnimationEvent] tile: {}", event.tile);
  on_delete_tile_animation(*mModel, event);
}

void App::_on_create_layer(const CreateLayerEvent& event)
{
  spdlog::trace("[CreateLayerEvent] type: {}", event.type);
  on_create_layer(*mModel, event);
}

void App::_on_remove_layer(const RemoveLayerEvent& event)
{
  spdlog::trace("[RemoveLayerEvent] layer: {}", event.layer);
  on_remove_layer(*mModel, event);
}

void App::_on_rename_layer(const RenameLayerEvent& event)
{
  spdlog::trace("[RenameLayerEvent] layer: {}, name: {}", event.layer, event.name);
  on_rename_layer(*mModel, event);
}

void App::_on_duplicate_layer(const DuplicateLayerEvent& event)
{
  spdlog::trace("[DuplicateLayerEvent] layer: {}", event.layer);
  on_duplicate_layer(*mModel, event);
}

void App::_on_select_layer(const SelectLayerEvent& event)
{
  spdlog::trace("[SelectLayerEvent] layer: {}", event.layer);
  on_select_layer(*mModel, event);
}

void App::_on_move_layer_up(const MoveLayerUpEvent& event)
{
  spdlog::trace("[MoveLayerUpEvent] layer: {}", event.layer);
  on_move_layer_up(*mModel, event);
}

void App::_on_move_layer_down(const MoveLayerDownEvent& event)
{
  spdlog::trace("[MoveLayerDownEvent] layer: {}", event.layer);
  on_move_layer_down(*mModel, event);
}

void App::_on_set_layer_opacity(const SetLayerOpacityEvent& event)
{
  spdlog::trace("[SetLayerOpacityEvent] layer: {}, opacity: {:.2f}",
                event.layer,
                event.opacity);
  on_set_layer_opacity(*mModel, event);
}

void App::_on_set_layer_visible(const SetLayerVisibleEvent& event)
{
  spdlog::trace("[SetLayerVisibleEvent] layer: {}, visible: {}",
                event.layer,
                event.visible);
  on_set_layer_visible(*mModel, event);
}

void App::_on_show_layer_rename_dialog(const ShowRenameLayerDialogEvent& event)
{
  spdlog::trace("[ShowRenameLayerDialogEvent] layer: {}", event.layer);
  on_show_rename_layer_dialog(*mModel, event);
}

void App::_on_move_object(const MoveObjectEvent& event)
{
  spdlog::trace("[MoveObjectEvent] object: {}, position: {}",
                event.object,
                event.new_pos);
  on_move_object(*mModel, event);
}

void App::_on_set_object_visible(const SetObjectVisibleEvent& event)
{
  spdlog::trace("[SetObjectVisibleEvent] object: {}, visible: {}",
                event.object,
                event.visible);
  on_set_object_visible(*mModel, event);
}

void App::_on_set_object_tag(const SetObjectTagEvent& event)
{
  spdlog::trace("[SetObjectTagEvent] object: {}, tag: {}", event.object, event.tag);
  on_set_object_tag(*mModel, event);
}

void App::_on_set_object_name(const SetObjectNameEvent& event)
{
  spdlog::trace("[SetObjectNameEvent] object: {}, name: {}", event.object, event.name);
  on_set_object_name(*mModel, event);
}

void App::_on_duplicate_object(const DuplicateObjectEvent& event)
{
  spdlog::trace("[DuplicateObjectEvent] object: {}", event.object);
  on_duplicate_object(*mModel, event);
}

void App::_on_remove_object(const RemoveObjectEvent& event)
{
  spdlog::trace("[RemoveObjectEvent] object layer: {}, object: {}",
                event.object_layer,
                event.object);
  on_remove_object(*mModel, event);
}

void App::_on_spawn_object_context_menu(const SpawnObjectContextMenuEvent& event)
{
  spdlog::trace("[SpawnObjectContextMenuEvent]");
  on_spawn_object_context_menu(*mModel, event);
}

void App::_on_stamp_sequence(const StampSequenceEvent& event)
{
  on_stamp_sequence(*mModel, event);
}

void App::_on_flood(const FloodEvent& event)
{
  on_flood(*mModel, event);
}

void App::_on_reload_fonts(const ReloadFontsEvent&)
{
  spdlog::trace("[ReloadFontsEvent]");
  mWantFontReload = true;
}

void App::_on_increase_font_size(const IncreaseFontSizeEvent& event)
{
  spdlog::trace("[IncreaseFontSizeEvent]");
  on_increase_font_size(*mModel, mDispatcher, event);
}

void App::_on_decrease_font_size(const DecreaseFontSizeEvent& event)
{
  spdlog::trace("[DecreaseFontSizeEvent]");
  on_decrease_font_size(*mModel, mDispatcher, event);
}

void App::_on_reset_font_size(const ResetFontSizeEvent& event)
{
  spdlog::trace("[ResetFontSizeEvent]");
  on_reset_font_size(*mModel, mDispatcher, event);
}

void App::_on_show_settings(const ShowSettingsEvent& event)
{
  spdlog::trace("[ShowSettingsEvent]");
  on_show_settings(*mModel, event);
}

void App::_on_set_settings(const SetSettingsEvent& event)
{
  spdlog::trace("[SetSettingsEvent]");
  on_set_settings(*mModel, mDispatcher, event);
}

void App::_on_set_flag_setting(const SetFlagSettingEvent& event)
{
  spdlog::trace("[SetFlagSettingEvent] flag: {}, value: {}", event.flag, event.value);
  on_set_flag_setting(*mModel, event);
}

void App::_on_negate_flag_setting(const NegateFlagSettingEvent& event)
{
  spdlog::trace("[NegateFlagSettingEvent] flag: {}", event.flag);
  on_negate_flag_setting(*mModel, event);
}

void App::_on_set_viewport_overlay_pos(const SetViewportOverlayPosEvent& event)
{
  spdlog::trace("[SetViewportOverlayPosEvent] pos: {}", event.pos);
  on_set_viewport_overlay_pos(*mModel, event);
}

void App::_on_set_language(const SetLanguageEvent& event)
{
  spdlog::trace("[SetLanguageEvent] lang: {}", event.language);
  on_set_language(*mModel, mDispatcher, event);
}

void App::_on_set_theme(const SetThemeEvent& event)
{
  spdlog::trace("[SetThemeEvent] theme: {}", event.theme);
  on_set_theme(*mModel, event);
}

void App::_on_reset_dock_visibilities(const ResetDockVisibilitiesEvent& event)
{
  spdlog::trace("[ResetDockVisibilitiesEvent]");
  on_reset_dock_visibilities(*mModel, event);
}

void App::_on_viewport_mouse_pressed(const ViewportMousePressedEvent& event)
{
  spdlog::trace("[ViewportMousePressedEvent]");
  on_viewport_mouse_pressed(*mModel, mDispatcher, event);
}

void App::_on_viewport_mouse_dragged(const ViewportMouseDraggedEvent& event)
{
  spdlog::trace("[ViewportMouseDraggedEvent]");
  on_viewport_mouse_dragged(*mModel, mDispatcher, event);
}

void App::_on_viewport_mouse_released(const ViewportMouseReleasedEvent& event)
{
  spdlog::trace("[ViewportMouseReleasedEvent]");
  on_viewport_mouse_released(*mModel, mDispatcher, event);
}

void App::_on_viewport_mouse_entered(const ViewportMouseEnteredEvent& event)
{
  spdlog::trace("[ViewportMouseEnteredEvent]");
  on_viewport_mouse_entered(*mModel, mDispatcher, event);
}

void App::_on_viewport_mouse_exited(const ViewportMouseExitedEvent& event)
{
  spdlog::trace("[ViewportMouseExitedEvent]");
  on_viewport_mouse_exited(*mModel, mDispatcher, event);
}

void App::_on_center_viewport(const CenterViewportEvent& event)
{
  spdlog::trace("[CenterViewportEvent] viewport: {}", event.viewport);
  on_center_viewport(*mModel, event);
}

void App::_on_reset_viewport_zoom(const ResetViewportZoomEvent& event)
{
  spdlog::trace("[ResetViewportZoomEvent] viewport: {}", event.viewport);
  on_reset_viewport_zoom(*mModel, event);
}

void App::_on_increase_viewport_zoom(const IncreaseViewportZoomEvent& event)
{
  spdlog::trace("[IncreaseViewportZoomEvent] viewport: {}", event.viewport);
  on_increase_viewport_zoom(*mModel, event);
}

void App::_on_decrease_viewport_zoom(const DecreaseViewportZoomEvent& event)
{
  spdlog::trace("[DecreaseViewportZoomEvent] viewport: {}", event.viewport);
  on_decrease_viewport_zoom(*mModel, event);
}

void App::_on_offset_viewport(const OffsetViewportEvent& event)
{
  spdlog::trace("[OffsetViewportEvent] viewport: {}, delta: {}",
                event.viewport,
                event.delta);
  on_offset_viewport(*mModel, event);
}

void App::_on_set_viewport_limits(const SetViewportLimitsEvent& event)
{
  spdlog::trace("[SetViewportLimitsEvent] viewport: {}, min: {}, max: {}",
                event.viewport,
                event.min_offset,
                event.max_offset);
  on_set_viewport_limits(*mModel, event);
}

void App::_on_set_dynamic_viewport_info(const SetDynamicViewportInfoEvent& event)
{
  // This event is usually dispatched multiple times for each frame, so we won't log it.
  on_set_dynamic_viewport_info(*mModel, event);
}

void App::_on_pan_viewport_up(const PanViewportUpEvent& event)
{
  spdlog::trace("[PanViewportUpEvent] viewport: {}", event.viewport);
  on_pan_viewport_up(*mModel, event);
}

void App::_on_pan_viewport_down(const PanViewportDownEvent& event)
{
  spdlog::trace("[PanViewportDownEvent] viewport: {}", event.viewport);
  on_pan_viewport_down(*mModel, event);
}

void App::_on_pan_viewport_left(const PanViewportLeftEvent& event)
{
  spdlog::trace("[PanViewportLeftEvent] viewport: {}", event.viewport);
  on_pan_viewport_left(*mModel, event);
}

void App::_on_pan_viewport_right(const PanViewportRightEvent& event)
{
  spdlog::trace("[PanViewportRightEvent] viewport: {}", event.viewport);
  on_pan_viewport_right(*mModel, event);
}

void App::_on_show_component_editor(const ShowComponentEditorEvent& event)
{
  spdlog::trace("[ShowComponentEditorEvent]");
  on_show_component_editor(*mModel, event);
}

void App::_on_show_new_comp_dialog(const ShowNewCompDialogEvent& event)
{
  spdlog::trace("[ShowNewCompDialogEvent]");
  on_show_new_comp_dialog(*mModel, event);
}

void App::_on_show_rename_comp_dialog(const ShowRenameCompDialogEvent& event)
{
  spdlog::trace("[ShowRenameCompDialogEvent]");
  on_show_rename_comp_dialog(*mModel, event);
}

void App::_on_show_new_comp_attr_dialog(const ShowNewCompAttrDialogEvent& event)
{
  spdlog::trace("[ShowNewComponentAttributeDialogEvent] component: {}", event.definition);
  on_show_new_comp_attr_dialog(*mModel, event);
}

void App::_on_show_rename_comp_attr_dialog(const ShowRenameCompAttrDialogEvent& event)
{
  spdlog::trace("[ShowRenameCompAttrDialogEvent] component: {}, attribute: {}",
                event.definition,
                event.attr_name);
  on_show_rename_comp_attr_dialog(*mModel, event);
}

void App::_on_define_component(const DefineComponentEvent& event)
{
  spdlog::trace("[DefineComponentEvent] name: {}", event.name);
  on_define_component(*mModel, event);
}

void App::_on_undef_component(const UndefComponentEvent& event)
{
  spdlog::trace("[UndefComponentEvent] component: {}", event.definition);
  on_undef_component(*mModel, event);
}

void App::_on_rename_component(const RenameComponentEvent& event)
{
  spdlog::trace("[RenameComponentEvent] component: {}, name: {}",
                event.definition,
                event.name);
  on_rename_component(*mModel, event);
}

void App::_on_update_component(const UpdateComponentEvent& event)
{
  spdlog::trace("[UpdateComponentEvent] component: {}, attribute: {}, value: {}",
                event.definition,
                event.attr_name,
                event.value);
  on_update_component(*mModel, event);
}

void App::_on_add_component_attr(const AddComponentAttrEvent& event)
{
  spdlog::trace("[AddComponentAttrEvent] component: {}, attribute: {}",
                event.definition,
                event.attr_name);
  on_add_component_attr(*mModel, event);
}

void App::_on_remove_component_attr(const RemoveComponentAttrEvent& event)
{
  spdlog::trace("[RemoveComponentAttrEvent] component: {}, attribute: {}",
                event.definition,
                event.attr_name);
  on_remove_component_attr(*mModel, event);
}

void App::_on_rename_component_attr(const RenameComponentAttrEvent& event)
{
  spdlog::trace("[RenameComponentAttrEvent] component: {}, old name: {}, new name: {}",
                event.definition,
                event.current_name,
                event.updated_name);
  on_rename_component_attr(*mModel, event);
}

void App::_on_duplicate_component_attr(const DuplicateComponentAttrEvent& event)
{
  spdlog::trace("[DuplicateComponentAttrEvent] component: {}, attribute: {}",
                event.definition,
                event.attr_name);
  on_duplicate_component_attr(*mModel, event);
}

void App::_on_set_component_attr_type(const SetComponentAttrTypeEvent& event)
{
  spdlog::trace("[SetComponentAttrTypeEvent] component: {}, attribute: {}, type: {}",
                event.definition,
                event.attr_name,
                event.type);
  on_set_component_attr_type(*mModel, event);
}

void App::_on_attach_component(const AttachComponentEvent& event)
{
  spdlog::trace("[AttachComponentEvent] context: {}, component: {}",
                event.context,
                event.definition);
  on_attach_component(*mModel, event);
}

void App::_on_detach_component(const DetachComponentEvent& event)
{
  spdlog::trace("[DetachComponentEvent] context: {}, component: {}",
                event.context,
                event.definition);
  on_detach_component(*mModel, event);
}

void App::_on_reset_attached_component(const ResetAttachedComponentEvent& event)
{
  spdlog::trace("[ResetAttachedComponentEvent] attached component: {}",
                event.attached_component);
  on_reset_attached_component(*mModel, event);
}

void App::_on_update_attached_component(const UpdateAttachedComponentEvent& event)
{
  spdlog::trace(
      "[UpdateAttachedComponentEvent] attached component: {}, attribute: {}, value: {}",
      event.attached_component,
      event.attr_name,
      event.value);
  on_update_attached_component(*mModel, event);
}

void App::_on_show_new_property_dialog(const ShowNewPropertyDialogEvent& event)
{
  spdlog::trace("[ShowNewPropertyDialogEvent] context: {}", event.context);
  on_show_new_property_dialog(*mModel, event);
}

void App::_on_show_rename_property_dialog(const ShowRenamePropertyDialogEvent& event)
{
  spdlog::trace("[ShowRenamePropertyDialogEvent] context: {}, property: {}",
                event.context,
                event.property_name);
  on_show_rename_property_dialog(*mModel, event);
}

void App::_on_show_set_property_type_dialog(const ShowSetPropertyTypeDialogEvent& event)
{
  spdlog::trace("[ShowSetPropertyTypeDialogEvent] context: {}, property: {}",
                event.context,
                event.property_name);
  on_show_set_property_type_dialog(*mModel, event);
}

void App::_on_inspect_context(const InspectContextEvent& event)
{
  spdlog::trace("[InspectContextEvent] context: {}", event.context);
  on_inspect_context(*mModel, event);
}

void App::_on_create_property(const CreatePropertyEvent& event)
{
  spdlog::trace("[CreatePropertyEvent] context: {}, name: {}, type: {}",
                event.context,
                event.name,
                event.type);
  on_create_property(*mModel, event);
}

void App::_on_remove_property(const RemovePropertyEvent& event)
{
  spdlog::trace("[RemovePropertyEvent] context: {}, name: {}", event.context, event.name);
  on_remove_property(*mModel, event);
}

void App::_on_rename_property(const RenamePropertyEvent& event)
{
  spdlog::trace("[RenamePropertyEvent] context: {}, old name: {}, new name: {}",
                event.context,
                event.old_name,
                event.new_name);
  on_rename_property(*mModel, event);
}

void App::_on_update_property(const UpdatePropertyEvent& event)
{
  spdlog::trace("[UpdatePropertyEvent] context: {}, name: {}, value: {}",
                event.context,
                event.name,
                event.value);
  on_update_property(*mModel, event);
}

void App::_on_set_property_type(const SetPropertyTypeEvent& event)
{
  spdlog::trace("[SetPropertyTypeEvent] context: {}, name: {}, type: {}",
                event.context,
                event.name,
                event.type);
  on_set_property_type(*mModel, event);
}

void App::_on_show_about_dialog(const ShowAboutDialogEvent&)
{
  spdlog::trace("[ShowAboutDialogEvent]");

  auto& widget_state = mModel->get<ui::WidgetState>();
  widget_state.about_dialog.should_open = true;
}

void App::_on_show_credits_dialog(const ShowCreditsDialogEvent&)
{
  spdlog::trace("[ShowCreditsDialog]");

  auto& widget_state = mModel->get<ui::WidgetState>();
  widget_state.credits_dialog.should_open = true;
}

void App::_on_show_about_imgui_dialog(const ShowAboutImGuiDialogEvent&)
{
  spdlog::trace("[ShowAboutImGuiDialogEvent]");

  auto& widget_state = mModel->get<ui::WidgetState>();
  widget_state.should_open_about_imgui_dialog = true;
}

void App::_on_open_directory_in_finder(const OpenDirectoryInFinderEvent& event)
{
  spdlog::trace("[OpenDirectoryInFinderEvent] dir: {}", event.dir);
  open_directory(event.dir);
}

void App::_on_open_url(const OpenUrlEvent& event)
{
  spdlog::trace("[OpenUrlEvent] URL: {}", event.url);
  cen::open_url(event.url);
}

}  // namespace tactile
