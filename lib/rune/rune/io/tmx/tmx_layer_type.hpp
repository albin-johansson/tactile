#ifndef RUNE_TMX_LAYER_TYPE_HPP
#define RUNE_TMX_LAYER_TYPE_HPP

#include <json.hpp>  // NLOHMANN_JSON_SERIALIZE_ENUM

namespace rune {

/// \addtogroup tmx
/// \{

enum class tmx_layer_type
{
  tile_layer,
  object_layer,
  image_layer,
  group
};

NLOHMANN_JSON_SERIALIZE_ENUM(tmx_layer_type,
                             {{tmx_layer_type::tile_layer, "tilelayer"},
                              {tmx_layer_type::image_layer, "imagelayer"},
                              {tmx_layer_type::object_layer, "objectgroup"},
                              {tmx_layer_type::group, "group"}})

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_LAYER_TYPE_HPP
