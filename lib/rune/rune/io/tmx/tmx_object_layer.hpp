#ifndef RUNE_TMX_OBJECT_LAYER_HPP
#define RUNE_TMX_OBJECT_LAYER_HPP

#include <cassert>   // assert
#include <json.hpp>  // NLOHMANN_JSON_SERIALIZE_ENUM
#include <vector>    // vector

#include "../../aliases/json_type.hpp"
#include "../json_utils.hpp"
#include "tmx_object.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

enum class tmx_object_layer_draw_order
{
  top_down,
  index
};

NLOHMANN_JSON_SERIALIZE_ENUM(tmx_object_layer_draw_order,
                             {{tmx_object_layer_draw_order::top_down, "topdown"},
                              {tmx_object_layer_draw_order::index, "index"}})

struct tmx_object_layer final  // Note, referred to as "object group" by tiled
{
  tmx_object_layer_draw_order draw_order{tmx_object_layer_draw_order::top_down};
  std::vector<tmx_object> objects;
};

inline void from_json(const json_type& json, tmx_object_layer& layer)
{
  io::get_to(json, "objects", layer.objects);
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_OBJECT_LAYER_HPP
