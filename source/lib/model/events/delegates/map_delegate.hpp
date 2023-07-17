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

#include "common/type/dispatcher.hpp"
#include "model/events/map_events.hpp"
#include "model/registry.hpp"

namespace tactile {

void on_show_new_map_dialog(Registry& registry, const ShowNewMapDialogEvent& event);

void on_show_open_map_dialog(Registry& registry, const ShowOpenMapDialogEvent& event);

void on_show_resize_map_dialog(Registry& registry, const ShowResizeMapDialogEvent& event);

void on_show_godot_export_dialog(Registry& registry,
                                 const ShowGodotExportDialogEvent& event);

void on_create_map(Registry& registry, const CreateMapEvent& event);

void on_open_map(Registry& registry, const OpenMapEvent& event);

void on_resize_map(Registry& registry, const ResizeMapEvent& event);

void on_add_row(Registry& registry, const AddRowEvent& event);

void on_add_column(Registry& registry, const AddColumnEvent& event);

void on_remove_row(Registry& registry, const RemoveRowEvent& event);

void on_remove_column(Registry& registry, const RemoveColumnEvent& event);

void on_fix_tiles_in_map(Registry& registry, const FixTilesInMapEvent& event);

void on_export_as_godot_scene(Registry& registry, const ExportAsGodotSceneEvent& event);

void on_inspect_map(Registry& registry, const InspectMapEvent& event);

void on_set_tile_format_encoding(Registry& registry,
                                 const SetTileFormatEncodingEvent& event);

void on_set_tile_format_compression(Registry& registry,
                                    const SetTileFormatCompressionEvent& event);

void on_set_zlib_compression_level(Registry& registry,
                                   const SetZlibCompressionLevelEvent& event);

void on_set_zstd_compression_level(Registry& registry,
                                   const SetZstdCompressionLevelEvent& event);

}  // namespace tactile
