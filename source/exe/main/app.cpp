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

#include <fmt/std.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "common/fmt/entity_formatter.hpp"
#include "common/fmt/lang_formatter.hpp"
#include "common/fmt/menu_action_formatter.hpp"
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
#include "model/context.hpp"
#include "model/delegates/command_delegate.hpp"
#include "model/delegates/file_delegate.hpp"
#include "model/delegates/font_delegate.hpp"
#include "model/delegates/input_delegate.hpp"
#include "model/delegates/map_delegate.hpp"
#include "model/delegates/menu_delegate.hpp"
#include "model/delegates/settings_delegate.hpp"
#include "model/delegates/viewport_delegate.hpp"
#include "model/systems/file_history_system.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/menu_system.hpp"
#include "model/systems/texture_system.hpp"
#include "model/systems/widget_system.hpp"
#include "ui/dialog/about_dialog.hpp"
#include "ui/dialog/create_map_dialog.hpp"
#include "ui/dialog/credits_dialog.hpp"
#include "ui/dialog/godot_export_dialog.hpp"
#include "ui/dialog/map_parse_error_dialog.hpp"
#include "ui/dialog/resize_map_dialog.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/animation/animation_dock.hpp"
#include "ui/dock/comp/component_dock.hpp"
#include "ui/dock/comp/component_editor.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/dock/layer/layer_dock.hpp"
#include "ui/dock/log/log_dock.hpp"
#include "ui/dock/property/property_dock.hpp"
#include "ui/dock/tileset/dialogs/create_tileset_dialog.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/menu/menu_bar.hpp"
#include "ui/shortcut/shortcuts.hpp"
#include "ui/style/colors.hpp"
#include "ui/style/themes.hpp"
#include "ui/viewport/viewport_widget.hpp"

namespace tactile {

void App::on_startup()
{
  auto& model = get_global_model();

  _subscribe_to_events();
  init_default_shortcuts();

  auto& icons = model.get<Icons>();
  icons.tactile_icon = sys::create_texture(model, find_resource("assets/icon.png"));

  _init_persistent_settings();

  sys::init_menus(model);
  _init_widgets();
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
  // clang-format on
}

void App::_init_persistent_settings()
{
  auto& model = get_global_model();

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

void App::_init_widgets()
{
  auto& model = get_global_model();

  uint32 idx = 0;
  sys::add_widget(model, idx++, ui::show_menu_bar);

  // Dock widgets
  sys::add_widget(model, idx++, &ui::show_layer_dock, &ui::is_layer_dock_enabled);
  sys::add_widget(model, idx++, &ui::show_tileset_dock, &ui::is_tileset_dock_enabled);
  sys::add_widget(model, idx++, &ui::show_animation_dock, &ui::is_animation_dock_enabled);
  sys::add_widget(model, idx++, &ui::show_property_dock);
  sys::add_widget(model, idx++, &ui::show_component_dock);
  sys::add_widget(model, idx++, &ui::show_log_dock);
  sys::add_widget(model, idx++, &ui::show_viewport_dock);

  // Dialogs
  sys::add_widget(model, idx++, &ui::show_component_editor_dialog);
  sys::add_widget(model, idx++, &ui::show_create_map_dialog);
  sys::add_widget(model, idx++, &ui::show_create_tileset_dialog);
  sys::add_widget(model, idx++, &ui::show_resize_map_dialog);
  sys::add_widget(model, idx++, &ui::show_godot_export_dialog);
  sys::add_widget(model, idx++, &ui::show_settings_dialog);
  sys::add_widget(model, idx++, &ui::show_map_parse_error_dialog);
  sys::add_widget(model, idx++, &ui::show_credits_dialog);
  sys::add_widget(model, idx, &ui::show_about_dialog);

  // TODO
  //  if (gOpenMapFileDialog) {
  //    auto dialog = FileDialog::open_map();
  //    if (dialog.is_okay()) {
  //      dispatcher.enqueue<OpenMapEvent>(dialog.path());
  //    }
  //  }

  // TODO
  //  if (gOpenAboutImGuiDialog) {
  //    center_next_window_on_appearance();
  //    ImGui::ShowAboutWindow(&gOpenAboutImGuiDialog);
  //  }

  sys::sort_widgets(model);
}

void App::on_shutdown()
{
  auto& model = get_global_model();

  sys::store_open_documents_in_file_history(model);
  save_settings_to_disk(model.get<Settings>());
  save_session_to_disk(model);

  const auto& file_history = model.get<FileHistory>();
  save_file_history_to_disk(file_history);
}

void App::on_update()
{
  auto& model = get_global_model();

  mDispatcher.update();

  // TODO update animated tiles
  sys::update_menu_items(model);

  ui::update_dynamic_color_cache();
  ui::update_dock_space(model);

  sys::render_widgets(model, mDispatcher);

  ui::check_for_missing_layout_file(model, mDispatcher);

  const auto& io = ImGui::GetIO();
  if (mFramebufferScale.x != io.DisplayFramebufferScale.x) {
    mFramebufferScale = io.DisplayFramebufferScale;
    mWantFontReload = true;
  }
}

void App::on_event(const cen::event_handler& event)
{
  tactile::on_event(get_global_model(), mDispatcher, event);
}

void App::on_font_reload()
{
  mWantFontReload = false;
}

void App::_on_menu_action(const MenuActionEvent& event)
{
  spdlog::trace("[MenuActionEvent] action: {}", event.action);
  on_menu_action(get_global_model(), mDispatcher, event);
}

void App::_on_undo(const UndoEvent& event)
{
  spdlog::trace("[UndoEvent]");
  on_undo(get_global_model(), event);
}

void App::_on_redo(const RedoEvent& event)
{
  spdlog::trace("[RedoEvent]");
  on_redo(get_global_model(), event);
}

void App::_on_set_command_capacity(const SetCommandCapacityEvent& event)
{
  spdlog::trace("[SetCommandCapacityEvent] capacity: {}", event.capacity);
  on_set_command_capacity(get_global_model(), event);
}

void App::_on_open_document(const OpenDocumentEvent& event)
{
  spdlog::trace("[OpenDocumentEvent] document: {}", event.document);
  on_open_document(get_global_model(), event);
}

void App::_on_close_document(const CloseDocumentEvent& event)
{
  spdlog::trace("[CloseDocumentEvent] document: {}", event.document);
  on_close_document(get_global_model(), event);
}

void App::_on_select_document(const SelectDocumentEvent& event)
{
  spdlog::trace("[SelectDocumentEvent] document: {}", event.document);
  on_select_document(get_global_model(), event);
}

void App::_on_save(const SaveEvent& event)
{
  spdlog::trace("[SaveEvent]");
  on_save(get_global_model(), mDispatcher, event);
}

void App::_on_save_as(const SaveAsEvent& event)
{
  spdlog::trace("[SaveAsEvent]");
  on_save_as(get_global_model(), mDispatcher, event);
}

void App::_on_show_save_as_dialog(const ShowSaveAsDialogEvent& event)
{
  spdlog::trace("[ShowSaveAsDialogEvent]");
  on_show_save_as_dialog(get_global_model(), mDispatcher, event);
}

void App::_on_quit(const QuitEvent&)
{
  spdlog::trace("[QuitEvent]");
  mShouldStop = true;
}

void App::_on_show_new_map_dialog(const ShowNewMapDialogEvent& event)
{
  spdlog::trace("[ShowNewMapDialogEvent]");
  on_show_new_map_dialog(get_global_model(), event);
}

void App::_on_show_open_map_dialog(const ShowOpenMapDialogEvent& event)
{
  spdlog::trace("[ShowOpenMapDialogEvent]");
  on_show_open_map_dialog(get_global_model(), event);
}

void App::_on_show_resize_map_dialog(const ShowResizeMapDialogEvent& event)
{
  spdlog::trace("[ShowResizeMapDialogEvent]");
  on_show_resize_map_dialog(get_global_model(), event);
}

void App::_on_create_map(const CreateMapEvent& event)
{
  spdlog::trace("[CreateMapEvent] rows: {}, columns: {}, tile size: {}",
                event.row_count,
                event.column_count,
                event.tile_size);
  on_create_map(get_global_model(), event);
}

void App::_on_open_map(const OpenMapEvent& event)
{
  spdlog::trace("[OpenMapEvent] path: {}", event.path);
  on_open_map(get_global_model(), event);
}

void App::_on_resize_map(const ResizeMapEvent& event)
{
  spdlog::trace("[ResizeMapEvent] rows: {}, cols: {}", event.row_count, event.col_count);
  on_resize_map(get_global_model(), event);
}

void App::_on_add_row(const AddRowEvent& event)
{
  spdlog::trace("[AddRowEvent]");
  on_add_row(get_global_model(), event);
}

void App::_on_add_column(const AddColumnEvent& event)
{
  spdlog::trace("[AddColumnEvent]");
  on_add_column(get_global_model(), event);
}

void App::_on_remove_row(const RemoveRowEvent& event)
{
  spdlog::trace("[RemoveRowEvent]");
  on_remove_row(get_global_model(), event);
}

void App::_on_remove_column(const RemoveColumnEvent& event)
{
  spdlog::trace("[RemoveColumnEvent]");
  on_remove_column(get_global_model(), event);
}

void App::_on_fix_tiles_in_map(const FixTilesInMapEvent& event)
{
  spdlog::trace("[FixTilesInMapEvent]");
  on_fix_tiles_in_map(get_global_model(), event);
}

void App::_on_export_as_godot_scene(const ExportAsGodotSceneEvent& event)
{
  spdlog::trace("[ExportAsGodotSceneEvent] root: {}", event.root_dir);
  on_export_as_godot_scene(get_global_model(), event);
}

void App::_on_inspect_map(const InspectMapEvent& event)
{
  spdlog::trace("[InspectMapEvent]");
  on_inspect_map(get_global_model(), event);
}

void App::_on_set_tile_format_encoding(const SetTileFormatEncodingEvent& event)
{
  spdlog::trace("[SetTileFormatEncodingEvent] encoding: {}", event.encoding);
  on_set_tile_format_encoding(get_global_model(), event);
}

void App::_on_set_tile_format_compression(const SetTileFormatCompressionEvent& event)
{
  spdlog::trace("[SetTileFormatCompressionEvent] compression: {}", event.compression);
  on_set_tile_format_compression(get_global_model(), event);
}

void App::_on_set_zlib_compression_level(const SetZlibCompressionLevelEvent& event)
{
  spdlog::trace("[SetZlibCompressionLevelEvent] level: {}", event.level);
  on_set_zlib_compression_level(get_global_model(), event);
}

void App::_on_set_zstd_compression_level(const SetZstdCompressionLevelEvent& event)
{
  spdlog::trace("[SetZstdCompressionLevelEvent] level: {}", event.level);
  on_set_zstd_compression_level(get_global_model(), event);
}

void App::_on_reload_fonts(const ReloadFontsEvent&)
{
  spdlog::trace("[ReloadFontsEvent]");
  mWantFontReload = true;
}

void App::_on_increase_font_size(const IncreaseFontSizeEvent& event)
{
  spdlog::trace("[IncreaseFontSizeEvent]");
  on_increase_font_size(get_global_model(), mDispatcher, event);
}

void App::_on_decrease_font_size(const DecreaseFontSizeEvent& event)
{
  spdlog::trace("[DecreaseFontSizeEvent]");
  on_decrease_font_size(get_global_model(), mDispatcher, event);
}

void App::_on_reset_font_size(const ResetFontSizeEvent& event)
{
  spdlog::trace("[ResetFontSizeEvent]");
  on_reset_font_size(get_global_model(), mDispatcher, event);
}

void App::_on_show_settings(const ShowSettingsEvent& event)
{
  spdlog::trace("[ShowSettingsEvent]");
  on_show_settings(get_global_model(), event);
}

void App::_on_set_settings(const SetSettingsEvent& event)
{
  spdlog::trace("[SetSettingsEvent]");
  on_set_settings(get_global_model(), mDispatcher, event);
}

void App::_on_set_flag_setting(const SetFlagSettingEvent& event)
{
  spdlog::trace("[SetFlagSettingEvent] flag: {}, value: {}", event.flag, event.value);
  on_set_flag_setting(get_global_model(), event);
}

void App::_on_negate_flag_setting(const NegateFlagSettingEvent& event)
{
  spdlog::trace("[NegateFlagSettingEvent] flag: {}", event.flag);
  on_negate_flag_setting(get_global_model(), event);
}

void App::_on_set_viewport_overlay_pos(const SetViewportOverlayPosEvent& event)
{
  spdlog::trace("[SetViewportOverlayPosEvent] pos: {}", magic_enum::enum_name(event.pos));
  on_set_viewport_overlay_pos(get_global_model(), event);
}

void App::_on_set_language(const SetLanguageEvent& event)
{
  spdlog::trace("[SetLanguageEvent] lang: {}", event.language);
  on_set_language(get_global_model(), mDispatcher, event);
}

void App::_on_set_theme(const SetThemeEvent& event)
{
  spdlog::trace("[SetThemeEvent] theme: {}", event.theme);
  on_set_theme(get_global_model(), event);
}

void App::_on_reset_dock_visibilities(const ResetDockVisibilitiesEvent& event)
{
  spdlog::trace("[ResetDockVisibilitiesEvent]");
  on_reset_dock_visibilities(get_global_model(), event);
}

void App::_on_viewport_mouse_pressed(const ViewportMousePressedEvent& event)
{
  spdlog::trace("[ViewportMousePressedEvent]");
  on_viewport_mouse_pressed(get_global_model(), mDispatcher, event);
}

void App::_on_viewport_mouse_dragged(const ViewportMouseDraggedEvent& event)
{
  spdlog::trace("[ViewportMouseDraggedEvent]");
  on_viewport_mouse_dragged(get_global_model(), mDispatcher, event);
}

void App::_on_viewport_mouse_released(const ViewportMouseReleasedEvent& event)
{
  spdlog::trace("[ViewportMouseReleasedEvent]");
  on_viewport_mouse_released(get_global_model(), mDispatcher, event);
}

void App::_on_viewport_mouse_entered(const ViewportMouseEnteredEvent& event)
{
  spdlog::trace("[ViewportMouseEnteredEvent]");
  on_viewport_mouse_entered(get_global_model(), mDispatcher, event);
}

void App::_on_viewport_mouse_exited(const ViewportMouseExitedEvent& event)
{
  spdlog::trace("[ViewportMouseExitedEvent]");
  on_viewport_mouse_exited(get_global_model(), mDispatcher, event);
}

}  // namespace tactile
