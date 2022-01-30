#pragma once

#include <functional>  // less
#include <map>         // map
#include <string>      // string
#include <vector>      // vector

#include "core/attribute_value.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/components/uv_tile_size.hpp"
#include "tactile_def.hpp"

namespace tactile::sys {

struct PropertyContextSnapshot final
{
  context_id id;
  std::string name;
  std::map<std::string, attribute_value, std::less<>> properties;
};

struct TilesetSnapshot final
{
  comp::tileset core;
  comp::tileset_selection selection;
  comp::texture texture;
  comp::uv_tile_size uv;
  PropertyContextSnapshot context;
};

struct ObjectSnapshot final
{
  comp::object core;
  PropertyContextSnapshot context;
};

struct LayerSnapshot final
{
  usize index{};
  comp::layer core;
  PropertyContextSnapshot context;
  maybe<layer_id> parent;
  maybe<tile_matrix> tiles;
  maybe<std::vector<ObjectSnapshot>> objects;
  maybe<std::vector<LayerSnapshot>> children;
};

}  // namespace tactile::sys