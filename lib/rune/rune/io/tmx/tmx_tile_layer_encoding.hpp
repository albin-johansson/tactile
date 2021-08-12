#ifndef RUNE_TMX_TILE_LAYER_ENCODING_HPP
#define RUNE_TMX_TILE_LAYER_ENCODING_HPP

#include <json.hpp>  // NLOHMANN_JSON_SERIALIZE_ENUM

namespace rune {

/// \addtogroup tmx
/// \{

enum class tmx_tile_layer_encoding
{
  csv,
  base64
};

NLOHMANN_JSON_SERIALIZE_ENUM(tmx_tile_layer_encoding,
                             {{tmx_tile_layer_encoding::csv, "csv"},
                              {tmx_tile_layer_encoding::base64, "base64"}})

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_TILE_LAYER_ENCODING_HPP
