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

#include <utility>  // move

#include "common/debug/panic.hpp"
#include "core/layer/group_layer.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/cmd/layer/all.hpp"
#include "model/cmd/map/all.hpp"
#include "model/cmd/object/all.hpp"
#include "model/cmd/tool/all.hpp"

namespace tactile {

MapDocument::MapDocument(const Int2& tile_size, const TileExtent extent)
    : mMap {std::make_shared<Map>()},
      mDelegate {mMap->get_uuid()}
{
  mMap->resize(extent);
  mMap->set_tile_size(tile_size);

  auto& context_manager = get_contexts();
  context_manager.add_context(mMap);
  context_manager.select_context(mMap->get_uuid());
}

void MapDocument::update()
{
  for (auto& [tileset_id, tileset_ref]: mMap->get_tileset_bundle()) {
    tileset_ref.update();
  }
}

void MapDocument::add_row()
{
  get_history().push<cmd::AddRow>(mMap);
}

void MapDocument::add_column()
{
  get_history().push<cmd::AddColumn>(mMap);
}

void MapDocument::remove_row()
{
  get_history().push<cmd::RemoveRow>(mMap);
}

void MapDocument::remove_column()
{
  get_history().push<cmd::RemoveColumn>(mMap);
}

void MapDocument::resize(const TileExtent extent)
{
  get_history().push<cmd::ResizeMap>(mMap, extent);
}

void MapDocument::fix_tiles()
{
  get_history().push<cmd::FixMapTiles>(mMap);
}

void MapDocument::add_layer(const LayerType type)
{
  get_history().push<cmd::AddLayer>(this, type);
}

void MapDocument::remove_layer(const UUID& layer_id)
{
  get_history().push<cmd::RemoveLayer>(this, layer_id);
}

void MapDocument::duplicate_layer(const UUID& layer_id)
{
  get_history().push<cmd::DuplicateLayer>(this, layer_id);
}

void MapDocument::rename_layer(const UUID& layer_id, String name)
{
  get_history().push<cmd::RenameLayer>(mMap, layer_id, std::move(name));
}

void MapDocument::move_layer_up(const UUID& layer_id)
{
  get_history().push<cmd::MoveLayerUp>(mMap, layer_id);
}

void MapDocument::move_layer_down(const UUID& layer_id)
{
  get_history().push<cmd::MoveLayerDown>(mMap, layer_id);
}

void MapDocument::set_layer_opacity(const UUID& layer_id, const float opacity)
{
  auto layer = mMap->get_invisible_root().find_shared_layer(layer_id);
  get_history().push<cmd::SetLayerOpacity>(std::move(layer), opacity);
}

void MapDocument::set_layer_visible(const UUID& layer_id, const bool visible)
{
  get_history().push<cmd::SetLayerVisible>(mMap, layer_id, visible);
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
  get_history().push<cmd::BucketFill>(mMap, layer_id, origin, replacement);
}

void MapDocument::add_rectangle(const UUID& layer_id,
                                const Float2& pos,
                                const Float2& size)
{
  get_history().push<cmd::AddObject>(this, layer_id, ObjectType::Rect, pos, size);
}

void MapDocument::add_ellipse(const UUID& layer_id, const Float2& pos, const Float2& size)
{
  get_history().push<cmd::AddObject>(this, layer_id, ObjectType::Ellipse, pos, size);
}

void MapDocument::add_point(const UUID& layer_id, const Float2& pos)
{
  get_history().push<cmd::AddObject>(this, layer_id, ObjectType::Point, pos);
}

void MapDocument::remove_object(const UUID& object_id)
{
  const auto layer_id = mMap->get_active_layer_id().value();
  get_history().push<cmd::RemoveObject>(this, layer_id, object_id);
}

void MapDocument::move_object(const UUID& object_id,
                              const Float2& previous,
                              const Float2& updated)
{
  auto object = get_object(object_id);
  get_history().push<cmd::MoveObject>(std::move(object), previous, updated);
}

void MapDocument::set_object_visible(const UUID& object_id, const bool visible)
{
  auto object = get_object(object_id);
  get_history().push<cmd::SetObjectVisible>(std::move(object), visible);
}

void MapDocument::set_object_name(const UUID& object_id, String name)
{
  auto object = get_object(object_id);
  get_history().push<cmd::RenameObject>(std::move(object), std::move(name));
}

void MapDocument::set_object_tag(const UUID& object_id, String tag)
{
  auto object = get_object(object_id);
  get_history().push<cmd::SetObjectTag>(std::move(object), std::move(tag));
}

auto MapDocument::get_object(const UUID& object_id) -> Shared<Object>
{
  auto& context_manager = get_contexts();
  const auto& context = context_manager.get_context_ptr(object_id);

  if (auto object = std::dynamic_pointer_cast<Object>(context)) {
    return object;
  }
  else {
    throw TactileError {"UUID did not reference an object"};
  }
}

void MapDocument::set_tile_format_encoding(const TileEncoding encoding)
{
  get_history().push<SetTileFormatEncoding>(mMap, encoding);
}

void MapDocument::set_tile_format_compression(const TileCompression compression)
{
  get_history().push<SetTileFormatCompression>(mMap, compression);
}

void MapDocument::set_zlib_compression_level(const int level)
{
  get_history().push<cmd::SetZlibCompressionLevel>(mMap, level);
}

void MapDocument::set_zstd_compression_level(const int level)
{
  get_history().push<cmd::SetZstdCompressionLevel>(mMap, level);
}

void MapDocument::set_component_index(Shared<ComponentIndex> index)
{
  mDelegate.set_component_index(std::move(index));
}

void MapDocument::set_name(String name)
{
  mMap->get_ctx().set_name(name);
}

void MapDocument::set_path(Path path)
{
  mDelegate.set_path(std::move(path));
}

auto MapDocument::has_path() const -> bool
{
  return mDelegate.has_path();
}

auto MapDocument::get_component_index_ptr() -> Shared<ComponentIndex>
{
  return mDelegate.get_component_index();
}

auto MapDocument::find_component_index() const -> const ComponentIndex*
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

auto MapDocument::get_name() const -> const String&
{
  return mMap->get_ctx().name();
}

auto MapDocument::get_path() const -> const Path&
{
  return mDelegate.path();
}

auto MapDocument::get_type() const -> DocumentType
{
  return DocumentType::Map;
}

}  // namespace tactile