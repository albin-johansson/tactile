#ifndef RUNE_TMX_VALIGN_HPP
#define RUNE_TMX_VALIGN_HPP

#include <json.hpp>  // NLOHMANN_JSON_SERIALIZE_ENUM

namespace rune {

/// \addtogroup tmx
/// \{

enum class tmx_valign
{
  center,
  top,
  bottom
};

NLOHMANN_JSON_SERIALIZE_ENUM(tmx_valign,
                             {{tmx_valign::center, "center"},
                              {tmx_valign::top, "top"},
                              {tmx_valign::bottom, "bottom"}})

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_VALIGN_HPP
