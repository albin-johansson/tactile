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

#include "tileset_document.hpp"

#include <utility>  // move

#include "core/tile/tile.hpp"
#include "core/tile/tile_matrix.hpp"
#include "core/tile/tileset_info.hpp"
#include "model/cmd/tile/all.hpp"
#include "model/cmd/tileset/all.hpp"

namespace tactile {

TilesetDocument::TilesetDocument(TilesetInfo info, const UUID& id)
    : mTileset {std::make_shared<Tileset>(std::move(info), id)},
      mDelegate {mTileset->get_uuid()}
{
  auto& context_manager = get_contexts();
  context_manager.add_context(mTileset);
  context_manager.select_context(mTileset->get_uuid());

  for (const auto& [tile_id, tile]: *mTileset) {
    context_manager.add_context(tile);
  }
}

void TilesetDocument::update()
{
  mTileset->update();
}

void TilesetDocument::rename_tileset(String name)
{
  get_history().push<cmd::RenameTileset>(mTileset, std::move(name));
}

void TilesetDocument::delete_animation(const TileIndex tile_index)
{
  get_history().push<cmd::DeleteAnimation>(this, tile_index);
}

void TilesetDocument::add_animation_frame(const TileIndex animated_tile_index,
                                          const TileIndex frame_tile_index,
                                          const ms_t frame_duration)
{
  get_history().push<cmd::AddAnimationFrame>(this,
                                             animated_tile_index,
                                             frame_tile_index,
                                             frame_duration);
}

void TilesetDocument::remove_animation_frame(const TileIndex tile_index,
                                             const usize frame_index)
{
  get_history().push<cmd::RemoveAnimationFrame>(this, tile_index, frame_index);
}

void TilesetDocument::move_animation_frame_forwards(const TileIndex tile_index,
                                                    const usize frame_index)
{
  get_history().push<cmd::MoveAnimationFrameForwards>(this, tile_index, frame_index);
}

void TilesetDocument::move_animation_frame_backwards(const TileIndex tile_index,
                                                     const usize frame_index)
{
  get_history().push<cmd::MoveAnimationFrameBackwards>(this, tile_index, frame_index);
}

void TilesetDocument::set_animation_frame_duration(const TileIndex tile_index,
                                                   const usize frame_index,
                                                   const ms_t frame_duration)
{
  get_history().push<cmd::SetAnimationFrameDuration>(this,
                                                     tile_index,
                                                     frame_index,
                                                     frame_duration);
}

void TilesetDocument::rename_tile(const TileIndex tile_index, String name)
{
  get_history().push<cmd::RenameTile>(this, tile_index, std::move(name));
}

void TilesetDocument::set_component_index(Shared<ComponentIndex> index)
{
  mDelegate.set_component_index(std::move(index));
}

void TilesetDocument::set_name(String name)
{
  mTileset->get_ctx().set_name(std::move(name));
}

void TilesetDocument::set_path(Path path)
{
  mDelegate.set_path(std::move(path));
}

auto TilesetDocument::has_path() const -> bool
{
  return mDelegate.has_path();
}

auto TilesetDocument::get_component_index_ptr() -> Shared<ComponentIndex>
{
  return mDelegate.get_component_index();
}

auto TilesetDocument::find_component_index() const -> const ComponentIndex*
{
  return mDelegate.view_component_index();
}

auto TilesetDocument::get_contexts() -> ContextManager&
{
  return mDelegate.get_contexts();
}

auto TilesetDocument::get_contexts() const -> const ContextManager&
{
  return mDelegate.get_contexts();
}

auto TilesetDocument::get_history() -> CommandStack&
{
  return mDelegate.get_history();
}

auto TilesetDocument::get_history() const -> const CommandStack&
{
  return mDelegate.get_history();
}

auto TilesetDocument::get_viewport() -> Viewport&
{
  return mDelegate.get_viewport();
}

auto TilesetDocument::get_viewport() const -> const Viewport&
{
  return mDelegate.get_viewport();
}

auto TilesetDocument::get_name() const -> const String&
{
  return mTileset->get_ctx().name();
}

auto TilesetDocument::get_path() const -> const Path&
{
  return mDelegate.path();
}

auto TilesetDocument::get_type() const -> DocumentType
{
  return DocumentType::Tileset;
}

}  // namespace tactile