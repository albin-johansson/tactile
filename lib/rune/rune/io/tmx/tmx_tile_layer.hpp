#ifndef RUNE_TMX_TILE_LAYER_HPP
#define RUNE_TMX_TILE_LAYER_HPP

#include "../../aliases/json_type.hpp"
#include "../../aliases/maybe.hpp"
#include "../json_utils.hpp"
#include "tmx_data.hpp"
#include "tmx_tile_layer_compression.hpp"
#include "tmx_tile_layer_encoding.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_tile_layer final
{
  tmx_tile_layer_compression compression{tmx_tile_layer_compression::none};
  tmx_tile_layer_encoding encoding{tmx_tile_layer_encoding::csv};
  maybe<tmx_data> data;
  // TODO std::vector<chunk> m_chunks;
};

inline void from_json(const json_type& json, tmx_tile_layer& layer)
{
  io::try_get_to(json, "compression", layer.compression);
  io::try_get_to(json, "encoding", layer.encoding);
  io::try_get_to(json, "data", layer.data);

  // TODO
  //  if (json.contains("chunks")) {
  //    m_chunks = detail::fill<std::vector<chunk>>(json, "chunks");
  //  }
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_TILE_LAYER_HPP
