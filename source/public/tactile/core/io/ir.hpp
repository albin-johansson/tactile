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

#include <filesystem>  // path
#include <string>      // string
#include <variant>     // variant
#include <vector>      // vector

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/layers/layer_type.hpp"
#include "tactile/core/layers/object_type.hpp"
#include "tactile/core/maps/tile_compression.hpp"
#include "tactile/core/maps/tile_encoding.hpp"

namespace tactile::ir {

enum ObjectRef : int32 {
};

struct Color final {
  uint8 red;
  uint8 green;
  uint8 blue;
  uint8 alpha;
};

struct Vec2 final {
  float32 x;
  float32 y;
};

struct Vec3 final {
  float32 x;
  float32 y;
  float32 z;
};

struct Vec4 final {
  float32 x;
  float32 y;
  float32 z;
  float32 w;
};

struct Vec2i final {
  int32 x;
  int32 y;
};

struct Vec3i final {
  int32 x;
  int32 y;
  int32 z;
};

struct Vec4i final {
  int32 x;
  int32 y;
  int32 z;
  int32 w;
};

using AttributeValue = std::variant<std::string,
                                    int32,
                                    float32,
                                    bool,
                                    Vec2,
                                    Vec3,
                                    Vec4,
                                    Vec2i,
                                    Vec3i,
                                    Vec4i,
                                    std::filesystem::path,
                                    ObjectRef,
                                    Color>;

struct Attribute final {
  std::string name;
  AttributeValue value;
};

struct AttachedComponent final {
  std::string name;
  std::vector<Attribute> attributes;
};

struct Metadata final {
  std::string name;
  std::vector<Attribute> properties;
  std::vector<AttachedComponent> components;
};

struct Object final {
  Metadata meta;
  ObjectID id;
  ObjectType type;
  float32 x;
  float32 y;
  float32 width;
  float32 height;
  std::string tag;
  bool visible;
};

struct Layer final {
  Metadata meta;
  LayerID id;
  LayerType type;
  std::vector<std::vector<TileID>> tiles;
  std::vector<Object> objects;
  std::vector<Layer> layers;
  float32 opacity;
  bool visible;
};

struct AnimationFrame final {
  TileIndex tile_index;
  uint64 duration_ms;
};

struct Tile final {
  Metadata meta;
  std::vector<Object> objects;
  std::vector<AnimationFrame> animation;
};

struct Tileset final {
  Metadata meta;
  TileID first_tile_id;
  int32 tile_width;
  int32 tile_height;
  usize row_count;
  usize col_count;
  int32 image_width;
  int32 image_height;
  std::filesystem::path image_path;
  std::vector<Tile> tiles;
};

struct TileFormat final {
  TileEncoding encoding;
  TileCompression compression;
  int32 zlib_level;
  int32 zstd_level;
};

struct Component final {
  std::string name;
  std::vector<Attribute> attributes;
};

struct Map final {
  Metadata meta;
  usize row_count;
  usize col_count;
  int32 tile_width;
  int32 tile_height;
  LayerID next_layer_id;
  ObjectID next_object_id;
  TileFormat format;
  std::vector<Component> components;
  std::vector<Tileset> tilesets;
  std::vector<Layer> layers;
};

}  // namespace tactile::ir
