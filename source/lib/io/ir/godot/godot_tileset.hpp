// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "common/util/assoc.hpp"
#include "io/ir/godot/godot_file.hpp"
#include "tactile/base/container/pair.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Information about a tileset in the merged tileset collection.
struct GdTilesetInfo final {
  UUID uuid;  /// Identifier of associated IR-tileset.

  String name;
  GdExtRes texture_ref {};
  TileID first_tile {};
  Int2 tile_size {};
  Int2 image_size {};
};

/// Intermediate representation of an amalgamated Godot tileset, stored in its own file.
class GodotTileset final : public GodotFile {
 public:
  using TextureNamePair = Pair<Path, String>;

  auto add_texture(const Path& dest, Path source) -> GdExtRes;

  void add_tileset(GdTilesetInfo info);

  [[nodiscard]] auto index_of(const UUID& tileset_id) const -> int32;

  [[nodiscard]] auto texture_paths() const -> const Vector<TextureNamePair>&;

  [[nodiscard]] auto tilesets() const -> const Vector<GdTilesetInfo>&;

 private:
  Vector<TextureNamePair> mSourceTexturePaths;
  Vector<GdTilesetInfo> mTilesetInfos;
};

}  // namespace tactile
