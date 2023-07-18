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

#include "map_delegate.hpp"

#include <fmt/std.h>

#include "cmd/map/add_column.hpp"
#include "cmd/map/add_row.hpp"
#include "cmd/map/fix_map_tiles.hpp"
#include "cmd/map/remove_column.hpp"
#include "cmd/map/remove_row.hpp"
#include "cmd/map/resize_map.hpp"
#include "cmd/map/set_tile_format_compression.hpp"
#include "cmd/map/set_tile_format_encoding.hpp"
#include "cmd/map/set_zlib_compression_level.hpp"
#include "cmd/map/set_zstd_compression_level.hpp"
#include "io/ir/godot/godot_converter.hpp"
#include "io/ir/godot/godot_options.hpp"
#include "io/ir/godot/godot_writer.hpp"
#include "io/ir/ir_generation.hpp"
#include "io/ir/ir_restoration.hpp"
#include "io/parsers/map_parser.hpp"
#include "model/documents/command_system.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/map_events.hpp"
#include "model/i18n/language_system.hpp"
#include "model/locator.hpp"
#include "model/maps/map_components.hpp"
#include "model/persistence/file_history_system.hpp"
#include "model/persistence/settings.hpp"
#include "model/persistence/settings_system.hpp"
#include "ui/dialog/resize_map_dialog.hpp"
#include "ui/widget_state.hpp"

namespace tactile {

void on_show_new_map_dialog(Registry& registry, const ShowNewMapDialogEvent&)
{
  const auto& settings_system = Locator<SettingsSystem>::get();
  const auto& settings = settings_system.current_settings();

  auto& widgets = registry.get<WidgetState>();
  widgets.new_map_dialog.tile_size = settings.get_preferred_tile_size();
  widgets.new_map_dialog.row_count = 5;
  widgets.new_map_dialog.col_count = 5;
  widgets.new_map_dialog.should_open = true;
}

void on_show_open_map_dialog(Registry& registry, const ShowOpenMapDialogEvent&)
{
  auto& widgets = registry.get<WidgetState>();
  widgets.should_open_map_file_dialog = true;
}

void on_show_resize_map_dialog(Registry& registry, const ShowResizeMapDialogEvent&)
{
  if (const auto* map = sys::try_get_active_map(registry)) {
    auto& widgets = registry.get<WidgetState>();
    widgets.resize_map_dialog.row_count = map->extent.rows;
    widgets.resize_map_dialog.col_count = map->extent.cols;
    widgets.resize_map_dialog.should_open = true;
  }
}

void on_show_godot_export_dialog(Registry& registry, const ShowGodotExportDialogEvent&)
{
  auto& widgets = registry.get<WidgetState>();
  widgets.godot_export_dialog.root_dir.clear();
  widgets.godot_export_dialog.map_dir.clear();
  widgets.godot_export_dialog.image_dir.clear();
  widgets.godot_export_dialog.tileset_dir.clear();
  widgets.godot_export_dialog.polygon_point_count = 16;
  widgets.godot_export_dialog.should_open = true;
}

void on_create_map(Registry& registry, const CreateMapEvent& event)
{
  sys::on_create_map(registry, event);
}

// TODO consider renaming event (if standalone tileset documents can be parsed)
void on_open_map(Registry& registry, const OpenMapEvent& event)
{
  const auto document_entity = sys::get_document_with_path(registry, event.path);

  if (document_entity != kNullEntity) {
    sys::open_document(registry, document_entity);
  }
  else {
    const auto ir_map = parse_map(event.path);
    if (ir_map.has_value()) {
      const auto absolute_document_path = fs::absolute(event.path);
      create_map_document_from_ir(*ir_map, absolute_document_path, registry);

      auto& file_history = registry.get<FileHistory>();
      sys::add_to_file_history(file_history, absolute_document_path);
    }
    else {
      const auto& strings = sys::get_current_language_strings(registry);
      auto& widget_state = registry.get<WidgetState>();

      auto& error_dialog = widget_state.map_parse_error_dialog;
      error_dialog.cause =
          fmt::format("{}: {}", strings.misc.cause, to_cause(strings, ir_map.error()));
      error_dialog.should_open = true;
    }
  }
}

void on_resize_map(Registry& registry, const ResizeMapEvent& event)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::ResizeMap>(registry,
                                     map_entity,
                                     TileExtent {event.row_count, event.col_count});
  }
}

void on_add_row(Registry& registry, const AddRowEvent&)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::AddRow>(registry, map_entity);
  }
}

void on_add_column(Registry& registry, const AddColumnEvent&)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::AddColumn>(registry, map_entity);
  }
}

void on_remove_row(Registry& registry, const RemoveRowEvent&)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::RemoveRow>(registry, map_entity);
  }
}

void on_remove_column(Registry& registry, const RemoveColumnEvent&)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::RemoveColumn>(registry, map_entity);
  }
}

void on_fix_tiles_in_map(Registry& registry, const FixTilesInMapEvent&)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::FixMapTiles>(registry, map_entity);
  }
}

void on_export_as_godot_scene(Registry& registry, const ExportAsGodotSceneEvent& event)
{
  if (sys::is_map_document_active(registry)) {
    const auto document_entity = sys::get_active_document(registry);
    const auto ir_map = convert_map_document_to_ir(registry, document_entity);

    const GodotEmitOptions options {
        .root_dir = event.root_dir,
        .project_map_dir = event.map_dir,
        .project_image_dir = event.image_dir,
        .project_tileset_dir = event.tileset_dir,
        .ellipse_polygon_point_count = event.polygon_points,
    };

    const auto godot_scene = convert_to_godot(ir_map, options);
    write_godot_scene(godot_scene, options);
  }
}

void on_inspect_map(Registry& registry, const InspectMapEvent&)
{
  if (sys::is_map_document_active(registry)) {
    const auto document_entity = sys::get_active_document(registry);
    const auto& map_document = registry.get<MapDocument>(document_entity);

    auto& document = registry.get<Document>(document_entity);
    document.active_context = map_document.map;
  }
}

void on_set_tile_format_encoding(Registry& registry,
                                 const SetTileFormatEncodingEvent& event)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::SetTileFormatEncoding>(registry, map_entity, event.encoding);
  }
}

void on_set_tile_format_compression(Registry& registry,
                                    const SetTileFormatCompressionEvent& event)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::SetTileFormatCompression>(registry,
                                                    map_entity,
                                                    event.compression);
  }
}

void on_set_zlib_compression_level(Registry& registry,
                                   const SetZlibCompressionLevelEvent& event)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::SetZlibCompressionLevel>(registry, map_entity, event.level);
  }
}

void on_set_zstd_compression_level(Registry& registry,
                                   const SetZstdCompressionLevelEvent& event)
{
  const auto map_entity = sys::get_active_map(registry);
  if (map_entity != kNullEntity) {
    sys::try_execute<cmd::SetZstdCompressionLevel>(registry, map_entity, event.level);
  }
}

}  // namespace tactile
