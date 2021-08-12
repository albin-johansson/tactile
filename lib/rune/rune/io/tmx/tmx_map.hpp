#ifndef RUNE_TMX_MAP_HPP
#define RUNE_TMX_MAP_HPP

#include <string>  // string

#include "../../aliases/maybe.hpp"
#include "tmx_color.hpp"
#include "tmx_layer.hpp"
#include "tmx_map_orientation.hpp"
#include "tmx_map_render_order.hpp"
#include "tmx_property.hpp"
#include "tmx_stagger_axis.hpp"
#include "tmx_stagger_index.hpp"
#include "tmx_tileset.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_map final
{
  int width{};   ///< Number of columns.
  int height{};  ///< Number of rows.

  int next_layer_id{1};
  int next_object_id{1};

  int tile_height{};
  int tile_width{};

  int compression_level{-1};
  int hex_side_length{};

  tmx_map_orientation orientation{tmx_map_orientation::orthogonal};
  tmx_map_render_order render_order{tmx_map_render_order::right_down};

  maybe<tmx_color> background;
  maybe<tmx_stagger_axis> stagger_axis;
  maybe<tmx_stagger_index> stagger_index;

  tmx_layers layers;
  tmx_tilesets tilesets;
  tmx_properties properties;

  std::string tiled_version;
  std::string json_version;
  bool infinite{};
};

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_MAP_HPP
