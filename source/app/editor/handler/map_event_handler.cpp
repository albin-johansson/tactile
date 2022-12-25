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

#include "map_event_handler.hpp"

#include <entt/signal/dispatcher.hpp>
#include <spdlog/spdlog.h>

#include "app/app_context.hpp"
#include "debug/assert.hpp"
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

void on_show_new_map_dialog()
{
  ui::open_create_map_dialog();
}

void on_show_open_map_dialog()
{
  ui::show_map_selector_dialog();
}

void on_create_map(const CreateMapEvent& event)
{
  auto& model = get_model();
  const auto id = model.add_map(event.tile_size, event.row_count, event.column_count);
  TACTILE_ASSERT(model.active_document_id() == id);
}

// TODO consider renaming event (when standalone tileset documents can be parsed)
void on_open_map(const OpenMapEvent& event)
{
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

void on_show_resize_map_dialog()
{
  if (auto* document = get_model().active_map()) {
    const auto& map = document->get_map();
    ui::open_resize_map_dialog(map.row_count(), map.column_count());
  }
}

void on_resize_map(const ResizeMapEvent& event)
{
  if (auto* map = get_model().active_map()) {
    map->resize(event.row_count, event.col_count);
  }
}

void on_add_row()
{
  if (auto* map = get_model().active_map()) {
    map->add_row();
  }
}

void on_add_column()
{
  if (auto* map = get_model().active_map()) {
    map->add_column();
  }
}

void on_remove_row()
{
  if (auto* map = get_model().active_map()) {
    map->remove_row();
  }
}

void on_remove_column()
{
  if (auto* map = get_model().active_map()) {
    map->remove_column();
  }
}

void on_fix_tiles()
{
  if (auto* map = get_model().active_map()) {
    map->fix_tiles();
  }
}

void on_export_as_godot_scene(const ExportAsGodotSceneEvent& event)
{
  if (auto* document = get_model().active_map()) {
    const io::GodotEmitOptions options {
        .root_dir = event.root_dir,
        .project_map_dir = event.map_dir,
        .project_image_dir = event.image_dir,
        .project_tileset_dir = event.tileset_dir,
        .ellipse_polygon_point_count = event.polygon_points,
    };
    io::emit_map_as_godot_scene(*document, options);
  }
}

void on_inspect_map()
{
  if (auto* document = get_model().active_map()) {
    const auto& map = document->get_map();
    document->get_contexts().select(map.get_uuid());
  }
}

void on_set_tile_format_encoding(SetTileFormatEncodingEvent event)
{
  if (auto* document = get_model().active_map()) {
    document->set_tile_format_encoding(event.encoding);
  }
}

void on_set_tile_format_compression(SetTileFormatCompressionEvent event)
{
  if (auto* document = get_model().active_map()) {
    document->set_tile_format_compression(event.compression);
  }
}

void on_set_zlib_compression_level(SetZlibCompressionLevelEvent event)
{
  if (auto* document = get_model().active_map()) {
    document->set_zlib_compression_level(event.level);
  }
}

void on_set_zstd_compression_level(SetZstdCompressionLevelEvent event)
{
  if (auto* document = get_model().active_map()) {
    document->set_zstd_compression_level(event.level);
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
