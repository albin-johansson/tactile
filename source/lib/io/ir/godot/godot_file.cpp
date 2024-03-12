// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "godot_file.hpp"

#include <utility>  // move

namespace tactile {

auto GodotFile::add_ext_resource(String path, String type) -> GdExtRes
{
  const auto id = mNextExtRes++;
  mExtResources[id] = GdExtResource {std::move(path), std::move(type)};
  return id;
}

auto GodotFile::add_atlas_texture(GdAtlasTexture texture) -> GdSubRes
{
  const auto id = mNextSubRes++;
  mAtlasTextures[id] = texture;
  return id;
}

void GodotFile::add_animation(GdAnimation animation)
{
  if (mSpriteFrames.animations.empty()) {
    mSpriteFrames.id = mNextSubRes++;
  }

  mSpriteFrames.animations.push_back(std::move(animation));
}

auto GodotFile::add_shape(GdRectShape shape) -> GdSubRes
{
  const auto id = mNextSubRes++;
  mRectShapes[id] = shape;
  return id;
}

void GodotFile::set_root_meta(GdMetaData meta)
{
  mRootMeta = std::move(meta);
}

auto GodotFile::get_load_steps() const -> usize
{
  return mExtResources.size() + static_cast<usize>(mNextSubRes - 1);
}

auto GodotFile::ext_resources() const -> const ExtResources&
{
  return mExtResources;
}

auto GodotFile::atlas_textures() const -> const AtlasTextures&
{
  return mAtlasTextures;
}

auto GodotFile::sprite_frames() const -> const GdSpriteFrames&
{
  return mSpriteFrames;
}

auto GodotFile::rectangle_shapes() const -> const RectShapes&
{
  return mRectShapes;
}

auto GodotFile::root_meta() const -> const GdMetaData&
{
  return mRootMeta;
}

}  // namespace tactile
