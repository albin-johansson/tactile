#pragma once

#include <filesystem>  // path
#include <memory>      // unique_ptr
#include <string>      // string
#include <variant>     // variant
#include <vector>      // vector

#include "core/attribute_value.hpp"
#include "core/layer_type.hpp"
#include "core/object_type.hpp"
#include "tactile_def.hpp"

namespace tactile::ir {

using component_map = tree_map<std::string, attribute_value>;

struct attribute_context_data final
{
  tree_map<std::string, attribute_value> properties;
  tree_map<std::string, component_map> components;
};

struct object_data final
{
  object_id id{};
  object_type type{};

  float x{};
  float y{};
  float width{};
  float height{};

  std::string name;
  std::string tag;

  attribute_context_data context;

  bool visible{};
};

struct tile_layer_data final
{
  std::vector<std::vector<tile_id>> tiles;
};

struct object_layer_data final
{
  std::vector<object_data> objects;
};

struct layer_data;

struct group_layer_data final
{
  std::vector<std::unique_ptr<layer_data>> children;
};

struct layer_data final
{
  using data_type = std::variant<tile_layer_data, object_layer_data, group_layer_data>;

  layer_id id{};
  layer_type type{};

  usize index{};

  std::string name;
  data_type data;

  attribute_context_data context;

  float opacity{};
  bool visible{};
};

struct fancy_animation_frame_data final
{
  tile_id local_id{};
  uint32 duration_ms{};
};

struct fancy_tile_data final
{
  std::vector<object_data> objects;
  std::vector<fancy_animation_frame_data> frames;
  attribute_context_data context;
};

struct tileset_data final
{
  std::string name;
  tile_id first_tile{};

  int32 tile_width{};
  int32 tile_height{};

  int32 tile_count{};
  int32 column_count{};

  std::filesystem::path image_path;
  int32 image_width{};
  int32 image_height{};

  hash_map<tile_id, fancy_tile_data> fancy_tiles;

  attribute_context_data context;
};

struct map_data
{
  usize row_count{};
  usize col_count{};

  int32 tile_width{};
  int32 tile_height{};

  int32 next_layer_id{};
  int32 next_object_id{};

  tree_map<std::string, component_map> component_definitions;

  std::vector<tileset_data> tilesets;
  std::vector<layer_data> layers;

  attribute_context_data context;
};

}  // namespace tactile::ir