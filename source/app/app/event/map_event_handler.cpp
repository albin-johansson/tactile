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

#include <entt/signal/dispatcher.hpp>
#include <fmt/std.h>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "app/app_context.hpp"
#include "app/event/event_handlers.hpp"
#include "common/util/fmt.hpp"
#include "core/debug/assert.hpp"
#include "io/map/emit/emitter.hpp"
#include "io/map/emit/gd/godot_options.hpp"
#include "io/map/ir/map_from_ir.hpp"
#include "io/map/parse/parse_map.hpp"
#include "io/proto/history.hpp"
#include "model/document/map_document.hpp"
#include "model/event/map_events.hpp"
#include "model/model.hpp"
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
  ui::show_map_selector_dialog();
}

void on_create_map(const CreateMapEvent& event)
{
  spdlog::trace("CreateMapEvent(tile_size: {}, row_count: {}, column_count: {})",
                event.tile_size,
                event.row_count,
                event.column_count);

  auto& model = get_model();
  const auto id =
      model.create_map_document(event.tile_size, event.row_count, event.column_count);
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
    const auto result = io::parse_map(event.path);
    if (result.error() == io::ParseError::None) {
      io::map_from_ir(result, model);
      io::add_file_to_history(event.path);
    }
    else {
      ui::open_map_parse_error_dialog(result.error());
    }
  }
}

void on_show_resize_map_dialog(const OpenResizeMapDialogEvent&)
{
  spdlog::trace("OpenResizeMapDialogEvent");

  if (auto* map_document = get_model().active_map_document()) {
    const auto& map = map_document->get_map();
    ui::open_resize_map_dialog(map.row_count(), map.column_count());
  }
}

void on_resize_map(const ResizeMapEvent& event)
{
  spdlog::trace("ResizeMapEvent(row_count: {}, col_count: {})",
                event.row_count,
                event.col_count);

  if (auto* map_document = get_model().active_map_document()) {
    map_document->resize(event.row_count, event.col_count);
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
    const io::GodotEmitOptions options {
        .root_dir = event.root_dir,
        .project_map_dir = event.map_dir,
        .project_image_dir = event.image_dir,
        .project_tileset_dir = event.tileset_dir,
        .ellipse_polygon_point_count = event.polygon_points,
    };
    io::emit_map_as_godot_scene(*map_document, options);
  }
}

void on_inspect_map(const InspectMapEvent&)
{
  spdlog::trace("InspectMapEvent");

  if (auto* map_document = get_model().active_map_document()) {
    const auto& map = map_document->get_map();
    map_document->get_contexts().select(map.get_uuid());
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
