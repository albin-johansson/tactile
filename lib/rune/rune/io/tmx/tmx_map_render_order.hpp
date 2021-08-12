#ifndef RUNE_TMX_MAP_RENDER_ORDER_HPP
#define RUNE_TMX_MAP_RENDER_ORDER_HPP

#include <json.hpp>  // NLOHMANN_JSON_SERIALIZE_ENUM

namespace rune {

/// \addtogroup tmx
/// \{

enum class tmx_map_render_order
{
  right_down,
  right_up,
  left_down,
  left_up
};

NLOHMANN_JSON_SERIALIZE_ENUM(tmx_map_render_order,
                             {{tmx_map_render_order::right_down, "right-down"},
                              {tmx_map_render_order::right_up, "right-up"},
                              {tmx_map_render_order::left_down, "left-down"},
                              {tmx_map_render_order::left_up, "left-up"}})

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_MAP_RENDER_ORDER_HPP
