// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "godot_tileset.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "common/util/filesystem.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

auto GodotTileset::add_texture(const Path& dest, Path source) -> GdExtRes
{
  const auto id =
      add_ext_resource(fmt::format("res://{}", use_forward_slashes(dest)), "Texture");
  mSourceTexturePaths.emplace_back(std::move(source), dest.filename().string());
  return id;
}

void GodotTileset::add_tileset(GdTilesetInfo info)
{
  mTilesetInfos.push_back(std::move(info));
}

auto GodotTileset::index_of(const UUID& tileset_id) const -> int32
{
  int32 index {};

  for (const auto& info: mTilesetInfos) {
    if (info.uuid == tileset_id) {
      return index;
    }

    ++index;
  }

  throw Exception {"Invalid tileset identifier!"};
}

auto GodotTileset::texture_paths() const -> const Vector<TextureNamePair>&
{
  return mSourceTexturePaths;
}

auto GodotTileset::tilesets() const -> const Vector<GdTilesetInfo>&
{
  return mTilesetInfos;
}

}  // namespace tactile
