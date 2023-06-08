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

#include <fmt/chrono.h>
#include <fmt/std.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include "common/fmt/entity_formatter.hpp"
#include "common/fmt/lang_formatter.hpp"
#include "common/fmt/layer_type_formatter.hpp"
#include "common/fmt/menu_action_formatter.hpp"
#include "common/fmt/overlay_pos_formatter.hpp"
#include "common/fmt/theme_formatter.hpp"
#include "common/fmt/tile_compression_formatter.hpp"
#include "common/fmt/tile_encoding_formatter.hpp"
#include "common/fmt/vector_formatter.hpp"
#include "components/file_history.hpp"
#include "components/texture.hpp"
#include "io/directories.hpp"
#include "io/proto/history.hpp"
#include "io/proto/session.hpp"
#include "io/proto/settings.hpp"
#include "model/delegates/command_delegate.hpp"
#include "model/delegates/component_delegate.hpp"
#include "model/delegates/file_delegate.hpp"
#include "model/delegates/font_delegate.hpp"
#include "model/delegates/input_delegate.hpp"
#include "model/delegates/layer_delegate.hpp"
#include "model/delegates/map_delegate.hpp"
#include "model/delegates/menu_delegate.hpp"
#include "model/delegates/settings_delegate.hpp"
#include "model/delegates/tileset_delegate.hpp"
#include "model/delegates/viewport_delegate.hpp"
#include "model/settings.hpp"
#include "model/systems/file_history_system.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/menu_system.hpp"
#include "model/systems/model_system.hpp"
#include "model/systems/texture_system.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/style/fonts.hpp"
#include "ui/style/themes.hpp"
#include "ui/ui.hpp"
#include "ui/widget_state.hpp"

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
  // clang-format off
  // Command events
  mDispatcher.sink<UndoEvent>().connect<&App::_on_undo>(this);
  mDispatcher.sink<RedoEvent>().connect<&App::_on_redo>(this);
  mDispatcher.sink<SetCommandCapacityEvent>().connect<&App::_on_set_command_capacity>(this);

  // File events
  mDispatcher.sink<OpenDocumentEvent>().connect<&App::_on_open_document>(this);
  mDispatcher.sink<CloseDocumentEvent>().connect<&App::_on_close_document>(this);
  mDispatcher.sink<SelectDocumentEvent>().connect<&App::_on_select_document>(this);
  mDispatcher.sink<SaveEvent>().connect<&App::_on_save>(this);
  mDispatcher.sink<SaveAsEvent>().connect<&App::_on_save_as>(this);
  mDispatcher.sink<ShowSaveAsDialogEvent>().connect<&App::_on_show_save_as_dialog>(this);
  mDispatcher.sink<QuitEvent>().connect<&App::_on_quit>(this);

  // Map events
  mDispatcher.sink<ShowNewMapDialogEvent>().connect<&App::_on_show_new_map_dialog>(this);
  mDispatcher.sink<ShowOpenMapDialogEvent>().connect<&App::_on_show_open_map_dialog>(this);
  mDispatcher.sink<ShowResizeMapDialogEvent>().connect<&App::_on_show_resize_map_dialog>(this);
  mDispatcher.sink<ShowGodotExportDialogEvent>().connect<&App::_on_show_godot_export_dialog>(this);
  mDispatcher.sink<CreateMapEvent>().connect<&App::_on_create_map>(this);
  mDispatcher.sink<OpenMapEvent>().connect<&App::_on_open_map>(this);
  mDispatcher.sink<ResizeMapEvent>().connect<&App::_on_resize_map>(this);
  mDispatcher.sink<AddRowEvent>().connect<&App::_on_add_row>(this);
  mDispatcher.sink<AddColumnEvent>().connect<&App::_on_add_column>(this);
  mDispatcher.sink<RemoveRowEvent>().connect<&App::_on_remove_row>(this);
  mDispatcher.sink<RemoveColumnEvent>().connect<&App::_on_remove_column>(this);
  mDispatcher.sink<FixTilesInMapEvent>().connect<&App::_on_fix_tiles_in_map>(this);
  mDispatcher.sink<ExportAsGodotSceneEvent>().connect<&App::_on_export_as_godot_scene>(this);
  mDispatcher.sink<InspectMapEvent>().connect<&App::_on_inspect_map>(this);
  mDispatcher.sink<SetTileFormatEncodingEvent>().connect<&App::_on_set_tile_format_encoding>(this);
  mDispatcher.sink<SetTileFormatCompressionEvent>().connect<&App::_on_set_tile_format_compression>(this);
  mDispatcher.sink<SetZlibCompressionLevelEvent>().connect<&App::_on_set_zlib_compression_level>(this);
  mDispatcher.sink<SetZstdCompressionLevelEvent>().connect<&App::_on_set_zstd_compression_level>(this);

  // Tileset events
  mDispatcher.sink<ShowNewTilesetDialogEvent>().connect<&App::_on_show_new_tileset_dialog>(this);
  mDispatcher.sink<CreateTilesetEvent>().connect<&App::_on_create_tileset>(this);
  mDispatcher.sink<DetachTilesetEvent>().connect<&App::_on_detach_tileset>(this);
  mDispatcher.sink<SelectTilesetEvent>().connect<&App::_on_select_tileset>(this);
  mDispatcher.sink<SetTilesetSelectionEvent>().connect<&App::_on_set_tileset_selection>(this);
  mDispatcher.sink<RenameTilesetEvent>().connect<&App::_on_rename_tileset>(this);
  mDispatcher.sink<SelectTilesetTileEvent>().connect<&App::_on_select_tileset_tile>(this);

  // Tile events
  mDispatcher.sink<AddAnimationFrameEvent>().connect<&App::_on_add_animation_frame>(this);
  mDispatcher.sink<SetAnimationFrameDurationEvent>().connect<&App::_on_set_animation_frame_duration>(this);
  mDispatcher.sink<EnableAnimationFrameSelectionMode>().connect<&App::_on_enable_animation_frame_selection_mode>(this);
  mDispatcher.sink<RemoveAnimationFrameEvent>().connect<&App::_on_remove_animation_frame>(this);
  mDispatcher.sink<MoveAnimationFrameForwardsEvent>().connect<&App::_on_move_animation_frame_forwards>(this);
  mDispatcher.sink<MoveAnimationFrameBackwardsEvent>().connect<&App::_on_move_animation_frame_backwards>(this);
  mDispatcher.sink<RenameTileEvent>().connect<&App::_on_rename_tile>(this);

  // Layer events
  mDispatcher.sink<CreateLayerEvent>().connect<&App::_on_create_layer>(this);
  mDispatcher.sink<RemoveLayerEvent>().connect<&App::_on_remove_layer>(this);
  mDispatcher.sink<RenameLayerEvent>().connect<&App::_on_rename_layer>(this);
  mDispatcher.sink<DuplicateLayerEvent>().connect<&App::_on_duplicate_layer>(this);
  mDispatcher.sink<SelectLayerEvent>().connect<&App::_on_select_layer>(this);
  mDispatcher.sink<MoveLayerUpEvent>().connect<&App::_on_move_layer_up>(this);
  mDispatcher.sink<MoveLayerDownEvent>().connect<&App::_on_move_layer_down>(this);
  mDispatcher.sink<SetLayerOpacityEvent>().connect<&App::_on_set_layer_opacity>(this);
  mDispatcher.sink<SetLayerVisibleEvent>().connect<&App::_on_set_layer_visible>(this);
  mDispatcher.sink<ShowLayerRenameDialogEvent>().connect<&App::_on_show_layer_rename_dialog>(this);

  // Font events
  mDispatcher.sink<ReloadFontsEvent>().connect<&App::_on_reload_fonts>(this);
  mDispatcher.sink<IncreaseFontSizeEvent>().connect<&App::_on_increase_font_size>(this);
  mDispatcher.sink<DecreaseFontSizeEvent>().connect<&App::_on_decrease_font_size>(this);
  mDispatcher.sink<ResetFontSizeEvent>().connect<&App::_on_reset_font_size>(this);

  // Setting events
  mDispatcher.sink<ShowSettingsEvent>().connect<&App::_on_show_settings>(this);
  mDispatcher.sink<SetSettingsEvent>().connect<&App::_on_set_settings>(this);
  mDispatcher.sink<SetFlagSettingEvent>().connect<&App::_on_set_flag_setting>(this);
  mDispatcher.sink<NegateFlagSettingEvent>().connect<&App::_on_negate_flag_setting>(this);
  mDispatcher.sink<SetViewportOverlayPosEvent>().connect<&App::_on_set_viewport_overlay_pos>(this);
  mDispatcher.sink<SetLanguageEvent>().connect<&App::_on_set_language>(this);
  mDispatcher.sink<SetThemeEvent>().connect<&App::_on_set_theme>(this);
  mDispatcher.sink<ResetDockVisibilitiesEvent>().connect<&App::_on_reset_dock_visibilities>(this);
  mDispatcher.sink<MenuActionEvent>().connect<&App::_on_menu_action>(this);

  // Viewport events
  mDispatcher.sink<ViewportMousePressedEvent>().connect<&App::_on_viewport_mouse_pressed>(this);
  mDispatcher.sink<ViewportMouseDraggedEvent>().connect<&App::_on_viewport_mouse_dragged>(this);
  mDispatcher.sink<ViewportMouseReleasedEvent>().connect<&App::_on_viewport_mouse_released>(this);
  mDispatcher.sink<ViewportMouseEnteredEvent>().connect<&App::_on_viewport_mouse_entered>(this);
  mDispatcher.sink<ViewportMouseExitedEvent>().connect<&App::_on_viewport_mouse_exited>(this);
  mDispatcher.sink<CenterViewportEvent>().connect<&App::_on_center_viewport>(this);
  mDispatcher.sink<ResetViewportZoomEvent>().connect<&App::_on_reset_viewport_zoom>(this);
  mDispatcher.sink<IncreaseViewportZoomEvent>().connect<&App::_on_increase_viewport_zoom>(this);
  mDispatcher.sink<DecreaseViewportZoomEvent>().connect<&App::_on_decrease_viewport_zoom>(this);
  mDispatcher.sink<OffsetViewportEvent>().connect<&App::_on_offset_viewport>(this);
  mDispatcher.sink<SetViewportLimitsEvent>().connect<&App::_on_set_viewport_limits>(this);
  mDispatcher.sink<SetDynamicViewportInfoEvent>().connect<&App::_on_set_dynamic_viewport_info>(this);
  mDispatcher.sink<PanViewportUpEvent>().connect<&App::_on_pan_viewport_up>(this);
  mDispatcher.sink<PanViewportDownEvent>().connect<&App::_on_pan_viewport_down>(this);
  mDispatcher.sink<PanViewportLeftEvent>().connect<&App::_on_pan_viewport_left>(this);
  mDispatcher.sink<PanViewportRightEvent>().connect<&App::_on_pan_viewport_right>(this);

  // Component events
  mDispatcher.sink<OpenComponentEditorEvent>().connect<&App::_on_open_component_editor>(this);
  // clang-format on
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

  auto& ui_state = model.get<ui::WidgetState>();
  ui::render_ui(model, ui_state, mDispatcher);
  ui::check_for_missing_layout_file(model, mDispatcher);

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

void App::_on_quit(const QuitEvent&)
{
  spdlog::trace("[QuitEvent]");
  mShouldStop = true;
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

void App::_on_show_layer_rename_dialog(const ShowLayerRenameDialogEvent& event)
{
  spdlog::trace("[ShowLayerRenameDialogEvent] layer: {}", event.layer);
  // TODO ui::show_rename_layer_dialog(event.layer);
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

void App::_on_open_component_editor(const OpenComponentEditorEvent& event)
{
  spdlog::trace("[OpenComponentEditorEvent]");
  on_open_component_editor(*mModel, event);
}

}  // namespace tactile
