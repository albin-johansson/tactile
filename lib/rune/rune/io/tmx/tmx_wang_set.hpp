#ifndef RUNE_TMX_WANG_SET_HPP
#define RUNE_TMX_WANG_SET_HPP

#include <string>  // string
#include <vector>  // vector

#include "../../aliases/json_type.hpp"
#include "../json_utils.hpp"
#include "tmx_local_id.hpp"
#include "tmx_property.hpp"
#include "tmx_wang_color.hpp"
#include "tmx_wang_tile.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_wang_set final
{
  tmx_local_id tile{};
  std::string name;
  std::vector<tmx_wang_tile> wang_tiles;
  std::vector<tmx_wang_color> colors;
  tmx_properties properties;
};

inline void from_json(const json_type& json, tmx_wang_set& set)
{
  io::emplace_to(json, "tile", set.tile);
  json.at("name").get_to(set.name);

  // TODO check if colors or wangtiles are required
  io::try_get_to(json, "colors", set.colors);
  io::try_get_to(json, "wangtiles", set.wang_tiles);
  io::try_get_to(json, "properties", set.properties);
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_WANG_SET_HPP
