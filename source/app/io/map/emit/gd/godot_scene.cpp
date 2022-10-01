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

#include "godot_scene.hpp"

#include <utility>  // move

#include "core/util/filesystem.hpp"
#include "core/util/fmt.hpp"

namespace tactile::io {

void GodotScene::set_tileset(GodotTileset tileset, const Path& dest)
{
  mTilesetId = add_ext_resource(format_str("res://{}", convert_to_forward_slashes(dest)),
                                "TileSet");
  mTileset = std::move(tileset);
}

void GodotScene::add_layer(GdLayer layer)
{
  mLayers.push_back(std::move(layer));
}

auto GodotScene::tileset_id() const -> GdExtRes
{
  return mTilesetId;
}

auto GodotScene::tileset() const -> const GodotTileset&
{
  return mTileset;
}

auto GodotScene::layers() const -> const Vec<GdLayer>&
{
  return mLayers;
}

}  // namespace tactile::io