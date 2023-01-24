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

#include "godot_file.hpp"

#include <utility>  // move

namespace tactile::io {

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

}  // namespace tactile::io
