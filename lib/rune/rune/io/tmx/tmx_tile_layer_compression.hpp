#ifndef RUNE_TMX_TILE_LAYER_COMPRESSION_HPP
#define RUNE_TMX_TILE_LAYER_COMPRESSION_HPP

#include <json.hpp>  // NLOHMANN_JSON_SERIALIZE_ENUM

namespace rune {

/// \addtogroup tmx
/// \{

enum class tmx_tile_layer_compression
{
  none,
  gzip,
  zlib
};

NLOHMANN_JSON_SERIALIZE_ENUM(tmx_tile_layer_compression,
                             {{tmx_tile_layer_compression::none, ""},
                              {tmx_tile_layer_compression::gzip, "gzip"},
                              {tmx_tile_layer_compression::zlib, "zlib"}})

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_TILE_LAYER_COMPRESSION_HPP
