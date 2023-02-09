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

#include <entt/signal/dispatcher.hpp>
#include <fmt/std.h>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "common/debug/assert.hpp"
#include "common/util/fmt.hpp"
#include "handlers/event_handlers.hpp"
#include "io/export/map_exporter.hpp"
#include "io/ir/map_from_ir.hpp"
#include "io/ir/map_to_ir.hpp"
#include "io/map/emit/gd/godot_converter.hpp"
#include "io/map/emit/gd/godot_options.hpp"
#include "io/map/emit/gd/godot_writer.hpp"
#include "io/map/parse/parse_map.hpp"
#include "model/document/map_document.hpp"
#include "model/event/map_events.hpp"
#include "model/file_history.hpp"
#include "model/model.hpp"
#include "runtime/app_context.hpp"
#include "ui/dialog/create_map_dialog.hpp"
#include "ui/dialog/map_parse_error_dialog.hpp"
#include "ui/dialog/resize_map_dialog.hpp"
#include "ui/ui.hpp"

namespace tactile {
namespace {

void on_show_new_map_dialog(const ShowNewMapDialogEvent&)
{
  spdlog::trace("ShowNewMapDialogEvent");
  ui::open_create_map_dialog();
}

void on_show_open_map_dialog(const ShowOpenMapDialogEvent&)
{
  spdlog::trace("ShowOpenMapDialogEvent");
  ui::open_map_file_dialog();
}

void on_create_map(const CreateMapEvent& event)
{
  spdlog::trace("CreateMapEvent(tile_size: {}, row_count: {}, column_count: {})",
                event.tile_size,
                event.row_count,
                event.column_count);

  auto& model = get_model();
  const auto id [[maybe_unused]] =
      model.create_map_document(event.tile_size,
                                TileExtent {event.row_count, event.column_count});
  TACTILE_ASSERT(model.get_active_document_id() == id);
}

// TODO consider renaming event (when standalone tileset documents can be parsed)
void on_open_map(const OpenMapEvent& event)
{
  spdlog::trace("OpenMapEvent(path: {})", event.path);

  auto& model = get_model();
  if (model.has_document_with_path(event.path)) {
    const auto id = model.get_id_for_path(event.path);

    if (model.is_open(id)) {
      spdlog::warn("Tried to open map that was already open!");
    }
    else {
      // FIXME this should be impossible (at least for map documents)? (verify)
      model.open_document(id);
    }
  }
  else {
    const auto ir_map = parse_map(event.path);
    if (ir_map.has_value()) {
      const auto absolute_document_path = fs::absolute(event.path);
      create_map_document_from_ir(*ir_map, absolute_document_path, model);
      add_to_file_history(absolute_document_path);
    }
    else {
      ui::open_map_parse_error_dialog(ir_map.error());
    }
  }
}

void on_show_resize_map_dialog(const OpenResizeMapDialogEvent&)
{
  spdlog::trace("OpenResizeMapDialogEvent");

  if (auto* map_document = get_model().active_map_document()) {
    const auto& map = map_document->get_map();
    ui::open_resize_map_dialog(map.get_extent());
  }
}

void on_resize_map(const ResizeMapEvent& event)
{
  spdlog::trace("ResizeMapEvent(row_count: {}, col_count: {})",
                event.row_count,
                event.col_count);

  if (auto* map_document = get_model().active_map_document()) {
    map_document->resize(TileExtent {event.row_count, event.col_count});
  }
}

void on_add_row(const AddRowEvent&)
{
  spdlog::trace("AddRowEvent");

  if (auto* map_document = get_model().active_map_document()) {
    map_document->add_row();
  }
}

void on_add_column(const AddColumnEvent&)
{
  spdlog::trace("AddColumnEvent");

  if (auto* map_document = get_model().active_map_document()) {
    map_document->add_column();
  }
}

void on_remove_row(const RemoveRowEvent&)
{
  spdlog::trace("RemoveRowEvent");

  if (auto* map_document = get_model().active_map_document()) {
    map_document->remove_row();
  }
}

void on_remove_column(const RemoveColumnEvent&)
{
  spdlog::trace("RemoveColumnEvent");

  if (auto* map_document = get_model().active_map_document()) {
    map_document->remove_column();
  }
}

void on_fix_tiles(const FixTilesInMapEvent&)
{
  spdlog::trace("FixTilesInMapEvent");

  if (auto* map_document = get_model().active_map_document()) {
    map_document->fix_tiles();
  }
}

void on_export_as_godot_scene(const ExportAsGodotSceneEvent& event)
{
  spdlog::trace(
      "ExportAsGodotSceneEvent"
      "(root_dir: {}, map_dir: {}, image_dir: {}, tileset_dir: {}, polygon_points: {})",
      event.root_dir,
      event.map_dir,
      event.image_dir,
      event.tileset_dir,
      event.polygon_points);

  if (auto* map_document = get_model().active_map_document()) {
    const GodotEmitOptions options {
        .root_dir = event.root_dir,
        .project_map_dir = event.map_dir,
        .project_image_dir = event.image_dir,
        .project_tileset_dir = event.tileset_dir,
        .ellipse_polygon_point_count = event.polygon_points,
    };

    const auto ir_map = convert_map_document_to_ir(*map_document);
    const auto godot_scene = convert_to_godot(ir_map, options);
    write_godot_scene(godot_scene, options);
  }
}

void on_inspect_map(const InspectMapEvent&)
{
  spdlog::trace("InspectMapEvent");

  if (auto* map_document = get_model().active_map_document()) {
    const auto& map = map_document->get_map();
    map_document->get_contexts().select_context(map.get_uuid());
  }
}

void on_set_tile_format_encoding(SetTileFormatEncodingEvent event)
{
  spdlog::trace("SetTileFormatEncodingEvent(encoding: {})",
                magic_enum::enum_name(event.encoding));

  if (auto* map_document = get_model().active_map_document()) {
    map_document->set_tile_format_encoding(event.encoding);
  }
}

void on_set_tile_format_compression(SetTileFormatCompressionEvent event)
{
  spdlog::trace("SetTileFormatCompressionEvent(compression: {})",
                magic_enum::enum_name(event.compression));

  if (auto* map_document = get_model().active_map_document()) {
    map_document->set_tile_format_compression(event.compression);
  }
}

void on_set_zlib_compression_level(SetZlibCompressionLevelEvent event)
{
  spdlog::trace("SetZlibCompressionLevelEvent(level: {})", event.level);

  if (auto* map_document = get_model().active_map_document()) {
    map_document->set_zlib_compression_level(event.level);
  }
}

void on_set_zstd_compression_level(SetZstdCompressionLevelEvent event)
{
  spdlog::trace("SetZstdCompressionLevelEvent(level: {})", event.level);

  if (auto* map_document = get_model().active_map_document()) {
    map_document->set_zstd_compression_level(event.level);
  }
}

}  // namespace

void install_map_event_handler()
{
  auto& dispatcher = get_dispatcher();

  dispatcher.sink<ShowNewMapDialogEvent>().connect<&on_show_new_map_dialog>();
  dispatcher.sink<ShowOpenMapDialogEvent>().connect<&on_show_open_map_dialog>();

  dispatcher.sink<CreateMapEvent>().connect<&on_create_map>();
  dispatcher.sink<OpenMapEvent>().connect<&on_open_map>();

  dispatcher.sink<OpenResizeMapDialogEvent>().connect<&on_show_resize_map_dialog>();
  dispatcher.sink<ResizeMapEvent>().connect<&on_resize_map>();

  dispatcher.sink<AddRowEvent>().connect<&on_add_row>();
  dispatcher.sink<AddColumnEvent>().connect<&on_add_column>();
  dispatcher.sink<RemoveRowEvent>().connect<&on_remove_row>();
  dispatcher.sink<RemoveColumnEvent>().connect<&on_remove_column>();

  dispatcher.sink<FixTilesInMapEvent>().connect<&on_fix_tiles>();
  dispatcher.sink<ExportAsGodotSceneEvent>().connect<&on_export_as_godot_scene>();

  dispatcher.sink<InspectMapEvent>().connect<&on_inspect_map>();

  // clang-format off
  dispatcher.sink<SetTileFormatEncodingEvent>().connect<&on_set_tile_format_encoding>();
  dispatcher.sink<SetTileFormatCompressionEvent>().connect<&on_set_tile_format_compression>();
  dispatcher.sink<SetZlibCompressionLevelEvent>().connect<&on_set_zlib_compression_level>();
  dispatcher.sink<SetZstdCompressionLevelEvent>().connect<&on_set_zstd_compression_level>();
  // clang-format on
}

}  // namespace tactile
