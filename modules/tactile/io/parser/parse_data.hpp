#pragma once

#include <filesystem>     // path
#include <functional>     // less, equal_to
#include <map>            // map
#include <memory>         // unique_ptr
#include <string>         // string
#include <unordered_map>  // unordered_map
#include <variant>        // variant
#include <vector>         // vector

#include "core/attribute_value.hpp"
#include "parse_error.hpp"
#include "tactile_def.hpp"

namespace tactile::parsing {

template <typename K, typename V>
using ordered_map = std::map<K, V, std::less<>>;

template <typename K, typename V>
using hash_map = std::unordered_map<K, V >;

using component_map = ordered_map<std::string, attribute_value>;

struct attribute_context_data final
{
  ordered_map<std::string, attribute_value> properties;
  ordered_map<std::string, component_map> components;
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
  TileMatrix tiles;
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

  ordered_map<std::string, component_map> component_definitions;

  std::vector<tileset_data> tilesets;
  std::vector<layer_data> layers;

  attribute_context_data context;
};

class parse_data final {
 public:
  void set_path(std::filesystem::path path);

  void set_error(parse_error error);

  [[nodiscard]] auto path() const -> const std::filesystem::path&;

  [[nodiscard]] auto error() const -> parse_error;

  [[nodiscard]] auto data() -> map_data&;

  [[nodiscard]] auto data() const -> const map_data&;

 private:
  std::filesystem::path mPath;
  map_data mData;
  parse_error mError{parse_error::none};
};

}  // namespace tactile::parsing