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

#include "core/attributes/attribute.hpp"
#include "tactile/core/containers/string.hpp"
#include "tactile/core/containers/string_map.hpp"
#include "tactile/core/containers/tree_map.hpp"
#include "tactile/core/containers/vector.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "core/math/vector.hpp"
#include "core/prelude.hpp"

namespace tactile {

using GdExtRes = int32;  ///< External resource identifier.
using GdSubRes = int32;  ///< Subresource identifier.

using GdAttributes = StringMap<Attribute>;

struct GdMetaData final {
  GdAttributes props;
  StringMap<GdAttributes> comps;
};

struct GdExtResource final {
  String path;
  String type;
};

struct GdAtlasTexture final {
  GdExtRes atlas_id {};
  Int4 region {};
};

struct GdAnimation final {
  String name;
  Vector<GdSubRes> frames;  /// Atlas textures
  float speed {};
};

struct GdSpriteFrames final {
  GdSubRes id {};
  Vector<GdAnimation> animations;
};

struct GdRectShape final {
  Float2 extents {};
};

class GodotFile {
 public:
  using ExtResources = TreeMap<GdExtRes, GdExtResource>;
  using AtlasTextures = TreeMap<GdSubRes, GdAtlasTexture>;
  using RectShapes = TreeMap<GdSubRes, GdRectShape>;

  auto add_ext_resource(String path, String type) -> GdExtRes;

  auto add_atlas_texture(GdAtlasTexture texture) -> GdSubRes;

  void add_animation(GdAnimation animation);

  auto add_shape(GdRectShape shape) -> GdSubRes;

  void set_root_meta(GdMetaData meta);

  [[nodiscard]] auto get_load_steps() const -> usize;

  [[nodiscard]] auto ext_resources() const -> const ExtResources&;

  [[nodiscard]] auto atlas_textures() const -> const AtlasTextures&;

  [[nodiscard]] auto sprite_frames() const -> const GdSpriteFrames&;

  [[nodiscard]] auto rectangle_shapes() const -> const RectShapes&;

  [[nodiscard]] auto root_meta() const -> const GdMetaData&;

 private:
  GdExtRes mNextExtRes {1};
  GdSubRes mNextSubRes {1};
  ExtResources mExtResources;
  AtlasTextures mAtlasTextures;
  RectShapes mRectShapes;
  GdSpriteFrames mSpriteFrames;
  GdMetaData mRootMeta;
};

}  // namespace tactile
