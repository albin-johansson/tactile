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

#include <centurion/event.hpp>
#include <imgui.h>

#include "common/type/dispatcher.hpp"
#include "engine/app_delegate.hpp"
#include "model/event/command_events.hpp"
#include "model/event/file_events.hpp"
#include "model/event/font_events.hpp"
#include "model/event/map_events.hpp"
#include "model/event/menu_events.hpp"
#include "model/event/setting_events.hpp"

namespace tactile {

/// The heart of the Tactile map editor.
class App final : public AppDelegate {
 public:
  void on_startup() override;

  void on_shutdown() override;

  void on_update() override;

  void on_event(const cen::event_handler& event) override;

  void on_font_reload() override;

  [[nodiscard]] auto want_font_reload() const -> bool override { return mWantFontReload; }
  [[nodiscard]] auto should_stop() const -> bool override { return mShouldStop; }

 private:
  Dispatcher mDispatcher;
  ImVec2 mFramebufferScale {};
  bool mShouldStop     : 1 {false};
  bool mWantFontReload : 1 {false};

  void _subscribe_to_events();
  void _init_persistent_settings();
  void _init_widgets();

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
  void _on_quit(const QuitEvent& event);

  // Map events
  void _on_show_new_map_dialog(const ShowNewMapDialogEvent& event);
  void _on_show_open_map_dialog(const ShowOpenMapDialogEvent& event);
  void _on_show_resize_map_dialog(const ShowResizeMapDialogEvent& event);
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
};

}  // namespace tactile
