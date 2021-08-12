#ifndef RUNE_TMX_STAGGER_INDEX_HPP
#define RUNE_TMX_STAGGER_INDEX_HPP

#include <json.hpp>  // NLOHMANN_JSON_SERIALIZE_ENUM

namespace rune {

/// \addtogroup tmx
/// \{

enum class tmx_stagger_index
{
  odd,
  even
};

NLOHMANN_JSON_SERIALIZE_ENUM(tmx_stagger_index,
                             {{tmx_stagger_index::odd, "odd"},
                              {tmx_stagger_index::even, "even"}})

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_STAGGER_INDEX_HPP
