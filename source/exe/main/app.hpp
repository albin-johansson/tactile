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

#pragma once

#include <centurion/events.hpp>
#include <imgui.h>

#include "common/type/dispatcher.hpp"
#include "engine/app_delegate.hpp"
#include "model/event/command_events.hpp"
#include "model/event/component_events.hpp"
#include "model/event/file_events.hpp"
#include "model/event/font_events.hpp"
#include "model/event/layer_events.hpp"
#include "model/event/map_events.hpp"
#include "model/event/menu_events.hpp"
#include "model/event/misc_events.hpp"
#include "model/event/object_events.hpp"
#include "model/event/property_events.hpp"
#include "model/event/setting_events.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/view_events.hpp"
#include "model/event/viewport_events.hpp"

namespace tactile {

/// The heart of the Tactile map editor.
class App final : public AppDelegate {
 public:
  void on_startup(BackendAPI api) override;

  void on_shutdown() override;

  void on_update() override;

  void on_event(const cen::event_handler& event) override;

  void reload_font_files() override;

  [[nodiscard]] auto want_font_reload() const -> bool override { return mWantFontReload; }

  [[nodiscard]] auto should_stop() const -> bool override { return mShouldStop; }

 private:
  Unique<Model> mModel;
  Dispatcher mDispatcher;
  ImVec2 mFramebufferScale {};
  bool mShouldStop     : 1 {false};
  bool mWantFontReload : 1 {false};

  void _subscribe_to_events();
  void _init_persistent_settings();

  void _on_menu_action(const MenuActionEvent& event);

  // Command events
  void _on_undo(const UndoEvent& event);
  void _on_redo(const RedoEvent& event);
  void _on_set_command_capacity(const SetCommandCapacityEvent& event);

  // File events
  void _on_open_document(const OpenDocumentEvent& event);
  void _on_close_document(const CloseDocumentEvent& event);
  void _on_select_document(const SelectDocumentEvent& event);
  void _on_save(const SaveEvent& event);
  void _on_save_as(const SaveAsEvent& event);
  void _on_show_save_as_dialog(const ShowSaveAsDialogEvent& event);
  void _on_reopen_last_closed_file(const ReopenLastClosedFileEvent& event);
  void _on_clear_file_history(const ClearFileHistoryEvent& event);
  void _on_quit(const QuitEvent& event);

  // View events
  void _on_reset_layout(const ResetLayoutEvent& event);
  void _on_toggle_highlight_layer(const ToggleHighlightLayerEvent& event);

  // Map events
  void _on_show_new_map_dialog(const ShowNewMapDialogEvent& event);
  void _on_show_open_map_dialog(const ShowOpenMapDialogEvent& event);
  void _on_show_resize_map_dialog(const ShowResizeMapDialogEvent& event);
  void _on_show_godot_export_dialog(const ShowGodotExportDialogEvent& event);
  void _on_create_map(const CreateMapEvent& event);
  void _on_open_map(const OpenMapEvent& event);
  void _on_resize_map(const ResizeMapEvent& event);
  void _on_add_row(const AddRowEvent& event);
  void _on_add_column(const AddColumnEvent& event);
  void _on_remove_row(const RemoveRowEvent& event);
  void _on_remove_column(const RemoveColumnEvent& event);
  void _on_fix_tiles_in_map(const FixTilesInMapEvent& event);
  void _on_export_as_godot_scene(const ExportAsGodotSceneEvent& event);
  void _on_inspect_map(const InspectMapEvent& event);
  void _on_set_tile_format_encoding(const SetTileFormatEncodingEvent& event);
  void _on_set_tile_format_compression(const SetTileFormatCompressionEvent& event);
  void _on_set_zlib_compression_level(const SetZlibCompressionLevelEvent& event);
  void _on_set_zstd_compression_level(const SetZstdCompressionLevelEvent& event);

  // Tileset events
  void _on_show_new_tileset_dialog(const ShowNewTilesetDialogEvent& event);
  void _on_create_tileset(const CreateTilesetEvent& event);
  void _on_detach_tileset(const DetachTilesetEvent& event);
  void _on_select_tileset(const SelectTilesetEvent& event);
  void _on_set_tileset_selection(const SetTilesetSelectionEvent& event);
  void _on_rename_tileset(const RenameTilesetEvent& event);
  void _on_select_tileset_tile(const SelectTilesetTileEvent& event);

  // Tile events
  void _on_add_animation_frame(const AddAnimationFrameEvent& event);
  void _on_set_animation_frame_duration(const SetAnimationFrameDurationEvent& event);
  void _on_enable_animation_frame_selection_mode(
      const EnableAnimationFrameSelectionMode& event);
  void _on_remove_animation_frame(const RemoveAnimationFrameEvent& event);
  void _on_move_animation_frame_forwards(const MoveAnimationFrameForwardsEvent& event);
  void _on_move_animation_frame_backwards(const MoveAnimationFrameBackwardsEvent& event);
  void _on_rename_tile(const RenameTileEvent& event);
  void _on_delete_tile_animation(const DeleteTileAnimationEvent& event);

  // Layer events
  void _on_create_layer(const CreateLayerEvent& event);
  void _on_remove_layer(const RemoveLayerEvent& event);
  void _on_rename_layer(const RenameLayerEvent& event);
  void _on_duplicate_layer(const DuplicateLayerEvent& event);
  void _on_select_layer(const SelectLayerEvent& event);
  void _on_move_layer_up(const MoveLayerUpEvent& event);
  void _on_move_layer_down(const MoveLayerDownEvent& event);
  void _on_set_layer_opacity(const SetLayerOpacityEvent& event);
  void _on_set_layer_visible(const SetLayerVisibleEvent& event);
  void _on_show_layer_rename_dialog(const ShowRenameLayerDialogEvent& event);

  // Object events
  void _on_move_object(const MoveObjectEvent& event);
  void _on_set_object_visible(const SetObjectVisibleEvent& event);
  void _on_set_object_tag(const SetObjectTagEvent& event);
  void _on_set_object_name(const SetObjectNameEvent& event);
  void _on_duplicate_object(const DuplicateObjectEvent& event);
  void _on_remove_object(const RemoveObjectEvent& event);
  void _on_spawn_object_context_menu(const SpawnObjectContextMenuEvent& event);

  // Font events
  void _on_reload_fonts(const ReloadFontsEvent& event);
  void _on_increase_font_size(const IncreaseFontSizeEvent& event);
  void _on_decrease_font_size(const DecreaseFontSizeEvent& event);
  void _on_reset_font_size(const ResetFontSizeEvent& event);

  // Setting events
  void _on_show_settings(const ShowSettingsEvent& event);
  void _on_set_settings(const SetSettingsEvent& event);
  void _on_set_flag_setting(const SetFlagSettingEvent& event);
  void _on_negate_flag_setting(const NegateFlagSettingEvent& event);
  void _on_set_viewport_overlay_pos(const SetViewportOverlayPosEvent& event);
  void _on_set_language(const SetLanguageEvent& event);
  void _on_set_theme(const SetThemeEvent& event);
  void _on_reset_dock_visibilities(const ResetDockVisibilitiesEvent& event);

  // Viewport events
  void _on_viewport_mouse_pressed(const ViewportMousePressedEvent& event);
  void _on_viewport_mouse_dragged(const ViewportMouseDraggedEvent& event);
  void _on_viewport_mouse_released(const ViewportMouseReleasedEvent& event);
  void _on_viewport_mouse_entered(const ViewportMouseEnteredEvent& event);
  void _on_viewport_mouse_exited(const ViewportMouseExitedEvent& event);
  void _on_center_viewport(const CenterViewportEvent& event);
  void _on_reset_viewport_zoom(const ResetViewportZoomEvent& event);
  void _on_increase_viewport_zoom(const IncreaseViewportZoomEvent& event);
  void _on_decrease_viewport_zoom(const DecreaseViewportZoomEvent& event);
  void _on_offset_viewport(const OffsetViewportEvent& event);
  void _on_set_viewport_limits(const SetViewportLimitsEvent& event);
  void _on_set_dynamic_viewport_info(const SetDynamicViewportInfoEvent& event);
  void _on_pan_viewport_up(const PanViewportUpEvent& event);
  void _on_pan_viewport_down(const PanViewportDownEvent& event);
  void _on_pan_viewport_left(const PanViewportLeftEvent& event);
  void _on_pan_viewport_right(const PanViewportRightEvent& event);

  // Component events
  void _on_show_component_editor(const ShowComponentEditorEvent& event);
  void _on_show_new_comp_dialog(const ShowNewCompDialogEvent& event);
  void _on_show_rename_comp_dialog(const ShowRenameCompDialogEvent& event);
  void _on_show_new_comp_attr_dialog(const ShowNewCompAttrDialogEvent& event);
  void _on_show_rename_comp_attr_dialog(const ShowRenameCompAttrDialogEvent& event);
  void _on_define_component(const DefineComponentEvent& event);
  void _on_undef_component(const UndefComponentEvent& event);
  void _on_rename_component(const RenameComponentEvent& event);
  void _on_update_component(const UpdateComponentEvent& event);
  void _on_add_component_attr(const AddComponentAttrEvent& event);
  void _on_remove_component_attr(const RemoveComponentAttrEvent& event);
  void _on_rename_component_attr(const RenameComponentAttrEvent& event);
  void _on_duplicate_component_attr(const DuplicateComponentAttrEvent& event);
  void _on_set_component_attr_type(const SetComponentAttrTypeEvent& event);
  void _on_attach_component(const AttachComponentEvent& event);
  void _on_detach_component(const DetachComponentEvent& event);
  void _on_reset_attached_component(const ResetAttachedComponentEvent& event);
  void _on_update_attached_component(const UpdateAttachedComponentEvent& event);

  // Property events
  void _on_show_new_property_dialog(const ShowNewPropertyDialogEvent& event);
  void _on_show_rename_property_dialog(const ShowRenamePropertyDialogEvent& event);
  void _on_show_set_property_type_dialog(const ShowSetPropertyTypeDialogEvent& event);
  void _on_inspect_context(const InspectContextEvent& event);
  void _on_create_property(const CreatePropertyEvent& event);
  void _on_remove_property(const RemovePropertyEvent& event);
  void _on_rename_property(const RenamePropertyEvent& event);
  void _on_update_property(const UpdatePropertyEvent& event);
  void _on_set_property_type(const SetPropertyTypeEvent& event);

  // Miscellaneous events
  void _on_show_about_dialog(const ShowAboutDialogEvent& event);
  void _on_show_credits_dialog(const ShowCreditsDialogEvent& event);
  void _on_show_about_imgui_dialog(const ShowAboutImGuiDialogEvent& event);
  void _on_open_directory_in_finder(const OpenDirectoryInFinderEvent& event);
  void _on_open_url(const OpenUrlEvent& event);
};

}  // namespace tactile
