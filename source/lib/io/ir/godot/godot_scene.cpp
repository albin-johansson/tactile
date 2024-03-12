// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "godot_scene.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "common/util/filesystem.hpp"

namespace tactile {

void GodotScene::set_tileset(GodotTileset tileset, const Path& dest)
{
  mTilesetId =
      add_ext_resource(fmt::format("res://{}", use_forward_slashes(dest)), "TileSet");
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

auto GodotScene::layers() const -> const Vector<GdLayer>&
{
  return mLayers;
}

}  // namespace tactile
