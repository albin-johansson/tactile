#ifndef RUNE_TMX_TILE_HPP
#define RUNE_TMX_TILE_HPP

#include <array>   // array
#include <string>  // string

#include "../../aliases/integers.hpp"
#include "../../aliases/json_type.hpp"
#include "../../aliases/maybe.hpp"
#include "../json_utils.hpp"
#include "core/from_string.hpp"
#include "tmx_animation.hpp"
#include "tmx_layer.hpp"
#include "tmx_local_id.hpp"
#include "tmx_property.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_tile final
{
  tmx_local_id id{};
  maybe<tmx_animation> animation;
  maybe<std::array<int, 4>> terrain;
  maybe<std::string> type;
  maybe<std::string> image;
  maybe<int> image_width;
  maybe<int> image_height;
  maybe<float> probability;
  maybe<tmx_layer> object_layer;
  tmx_properties properties;
};

inline void from_json(const json_type& json, tmx_tile& tile)
{
  io::emplace_to(json, "id", tile.id);

  io::try_get_to(json, "animation", tile.animation);
  io::try_get_to(json, "type", tile.type);
  io::try_get_to(json, "image", tile.image);
  io::try_get_to(json, "imagewidth", tile.image_width);
  io::try_get_to(json, "imageheight", tile.image_height);
  io::try_get_to(json, "probability", tile.probability);
  io::try_get_to(json, "objectgroup", tile.object_layer);
  io::try_get_to(json, "properties", tile.properties);

  if (const auto it = json.find("terrain"); it != json.end())
  {
    auto& terrain = tile.terrain.emplace();
    for (const auto& [key, value] : it->items())
    {
      const auto index = from_string<usize>(key).value();
      terrain.at(index) = value.get<int>();
    }
  }
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_TILE_HPP
