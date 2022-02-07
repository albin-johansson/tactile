#pragma once

#include <string>  // string
#include <vector>  // vector

#include "core/attribute.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/components/uv_tile_size.hpp"
#include "tactile.hpp"

namespace tactile::sys {

struct component_snapshot final
{
  tree_map<std::string, attribute_value> attributes;
};

struct attribute_context_snapshot final
{
  context_id id{};
  std::string name;
  tree_map<std::string, attribute_value> properties;
  tree_map<component_id, component_snapshot> components;
};

struct TilesetSnapshot final
{
  comp::tileset core;
  comp::tileset_selection selection;
  comp::texture texture;
  comp::uv_tile_size uv;
  attribute_context_snapshot context;
};

struct ObjectSnapshot final
{
  comp::object core;
  attribute_context_snapshot context;
};

struct LayerSnapshot final
{
  usize index{};
  comp::layer core;
  attribute_context_snapshot context;
  maybe<layer_id> parent;
  maybe<tile_matrix> tiles;
  maybe<std::vector<ObjectSnapshot>> objects;
  maybe<std::vector<LayerSnapshot>> children;
};

}  // namespace tactile::sys