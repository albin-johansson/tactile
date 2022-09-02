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

#include "map_document.hpp"

#include <algorithm>  // find_if
#include <utility>    // move

#include "core/cmd/command_stack.hpp"
#include "core/cmd/layer/all.hpp"
#include "core/cmd/map/all.hpp"
#include "core/cmd/object/all.hpp"
#include "core/cmd/tool/all.hpp"
#include "misc/panic.hpp"

namespace tactile {

MapDocument::MapDocument(const int2& tile_size, const usize rows, const usize columns)
    : mMap {std::make_shared<Map>()}
    , mDelegate {mMap->get_uuid()}
{
  mMap->resize(rows, columns);
  mMap->set_tile_size(tile_size);

  auto& contexts = get_contexts();
  contexts.add_context(mMap);
  contexts.select(mMap->get_uuid());
}

void MapDocument::update()
{
  for (auto& [id, ref] : mMap->get_tilesets()) {
    ref.update();
  }
}

void MapDocument::add_row()
{
  get_history().exec<cmd::AddRow>(mMap);
}

void MapDocument::add_column()
{
  get_history().exec<cmd::AddColumn>(mMap);
}

void MapDocument::remove_row()
{
  get_history().exec<cmd::RemoveRow>(mMap);
}

void MapDocument::remove_column()
{
  get_history().exec<cmd::RemoveColumn>(mMap);
}

void MapDocument::resize(const usize rows, const usize cols)
{
  get_history().exec<cmd::ResizeMap>(mMap, rows, cols);
}

void MapDocument::fix_tiles()
{
  get_history().exec<cmd::FixMapTiles>(mMap);
}

void MapDocument::add_layer(const LayerType type)
{
  get_history().exec<cmd::AddLayer>(this, type);
}

void MapDocument::remove_layer(const UUID& layer_id)
{
  get_history().exec<cmd::RemoveLayer>(this, layer_id);
}

void MapDocument::duplicate_layer(const UUID& layer_id)
{
  get_history().exec<cmd::DuplicateLayer>(this, layer_id);
}

void MapDocument::rename_layer(const UUID& layer_id, std::string name)
{
  get_history().exec<cmd::RenameLayer>(mMap, layer_id, std::move(name));
}

void MapDocument::move_layer_up(const UUID& layer_id)
{
  get_history().exec<cmd::MoveLayerUp>(mMap, layer_id);
}

void MapDocument::move_layer_down(const UUID& layer_id)
{
  get_history().exec<cmd::MoveLayerDown>(mMap, layer_id);
}

void MapDocument::set_layer_opacity(const UUID& layer_id, const float opacity)
{
  auto layer = mMap->get_layer(layer_id);
  get_history().exec<cmd::SetLayerOpacity>(std::move(layer), opacity);
}

void MapDocument::set_layer_visible(const UUID& layer_id, const bool visible)
{
  get_history().exec<cmd::SetLayerVisible>(mMap, layer_id, visible);
}

void MapDocument::register_stamp_sequence(const UUID& layer_id,
                                          TileCache previous,
                                          TileCache sequence)
{
  get_history().store<cmd::StampSequence>(mMap,
                                          layer_id,
                                          std::move(previous),
                                          std::move(sequence));
}

void MapDocument::register_eraser_sequence(const UUID& layer_id, TileCache previous)
{
  get_history().store<cmd::EraserSequence>(mMap, layer_id, std::move(previous));
}

void MapDocument::flood(const UUID& layer_id,
                        const TilePos& origin,
                        const TileID replacement)
{
  get_history().exec<cmd::BucketFill>(mMap, layer_id, origin, replacement);
}

void MapDocument::add_rectangle(const UUID& layer_id,
                                const float2& pos,
                                const float2& size)
{
  get_history().exec<cmd::AddObject>(this, layer_id, ObjectType::Rect, pos, size);
}

void MapDocument::add_ellipse(const UUID& layer_id, const float2& pos, const float2& size)
{
  get_history().exec<cmd::AddObject>(this, layer_id, ObjectType::Ellipse, pos, size);
}

void MapDocument::add_point(const UUID& layer_id, const float2& pos)
{
  get_history().exec<cmd::AddObject>(this, layer_id, ObjectType::Point, pos);
}

void MapDocument::remove_object(const UUID& object_id)
{
  const auto layer_id = mMap->active_layer_id().value();
  get_history().exec<cmd::RemoveObject>(this, layer_id, object_id);
}

void MapDocument::move_object(const UUID& object_id,
                              const float2& previous,
                              const float2& updated)
{
  auto object = get_object(object_id);
  get_history().exec<cmd::MoveObject>(std::move(object), previous, updated);
}

void MapDocument::set_object_visible(const UUID& object_id, const bool visible)
{
  auto object = get_object(object_id);
  get_history().exec<cmd::SetObjectVisible>(std::move(object), visible);
}

void MapDocument::set_object_name(const UUID& object_id, std::string name)
{
  auto object = get_object(object_id);
  get_history().exec<cmd::RenameObject>(std::move(object), std::move(name));
}

void MapDocument::set_object_tag(const UUID& object_id, std::string tag)
{
  auto object = get_object(object_id);
  get_history().exec<cmd::SetObjectTag>(std::move(object), std::move(tag));
}

auto MapDocument::get_object(const UUID& object_id) -> Shared<Object>
{
  auto& contexts = get_contexts();
  if (auto ptr = std::dynamic_pointer_cast<Object>(contexts.get_context(object_id))) {
    return ptr;
  }
  else {
    throw TactileError {"Did not find object!"};
  }
}

void MapDocument::set_tile_format_encoding(const TileEncoding encoding)
{
  get_history().exec<SetTileFormatEncoding>(mMap, encoding);
}

void MapDocument::set_tile_format_compression(const TileCompression compression)
{
  get_history().exec<SetTileFormatCompression>(mMap, compression);
}

void MapDocument::set_zlib_compression_level(const int level)
{
  get_history().exec<cmd::SetZlibCompressionLevel>(mMap, level);
}

void MapDocument::set_zstd_compression_level(const int level)
{
  get_history().exec<cmd::SetZstdCompressionLevel>(mMap, level);
}

void MapDocument::set_component_index(Shared<ComponentIndex> index)
{
  mDelegate.set_component_index(std::move(index));
}

void MapDocument::set_name(std::string name)
{
  mMap->set_name(name);
}

void MapDocument::set_path(fs::path path)
{
  mDelegate.set_path(std::move(path));
}

auto MapDocument::has_path() const -> bool
{
  return mDelegate.has_path();
}

auto MapDocument::get_component_index() -> Shared<ComponentIndex>
{
  return mDelegate.get_component_index();
}

auto MapDocument::view_component_index() const -> const ComponentIndex*
{
  return mDelegate.view_component_index();
}

auto MapDocument::get_tools() -> ToolManager&
{
  return mTools;
}

auto MapDocument::get_tools() const -> const ToolManager&
{
  return mTools;
}

auto MapDocument::get_contexts() -> ContextManager&
{
  return mDelegate.get_contexts();
}

auto MapDocument::get_contexts() const -> const ContextManager&
{
  return mDelegate.get_contexts();
}

auto MapDocument::get_history() -> CommandStack&
{
  return mDelegate.get_history();
}

auto MapDocument::get_history() const -> const CommandStack&
{
  return mDelegate.get_history();
}

auto MapDocument::get_viewport() -> Viewport&
{
  return mDelegate.get_viewport();
}

auto MapDocument::get_viewport() const -> const Viewport&
{
  return mDelegate.get_viewport();
}

auto MapDocument::get_name() const -> const std::string&
{
  return mMap->get_name();
}

auto MapDocument::get_path() const -> const fs::path&
{
  return mDelegate.path();
}

auto MapDocument::get_type() const -> DocumentType
{
  return DocumentType::Map;
}

}  // namespace tactile