#ifndef RUNE_TMX_WANG_TILE_HPP
#define RUNE_TMX_WANG_TILE_HPP

#include <array>  // array

#include "../../aliases/integers.hpp"
#include "../../aliases/json_type.hpp"
#include "../json_utils.hpp"
#include "tmx_local_id.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_wang_tile final
{
  tmx_local_id tile{};
  std::array<uint8, 8> indices{};
};

inline void from_json(const json_type& json, tmx_wang_tile& tile)
{
  io::emplace_to(json, "tileid", tile.tile);
  json.at("wangid").get_to(tile.indices);
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_WANG_TILE_HPP
