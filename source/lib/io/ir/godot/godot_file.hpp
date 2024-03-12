// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/attribute.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/string_map.hpp"
#include "tactile/base/container/tree_map.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/vec.hpp"

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
