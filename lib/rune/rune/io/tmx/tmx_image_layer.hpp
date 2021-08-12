#ifndef RUNE_TMX_IMAGE_LAYER_HPP
#define RUNE_TMX_IMAGE_LAYER_HPP

#include <string>  // string

#include "../../aliases/json_type.hpp"
#include "../../aliases/maybe.hpp"
#include "../json_utils.hpp"
#include "tmx_color.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_image_layer final
{
  std::string image;
  maybe<tmx_color> transparent;
};

inline void from_json(const json_type& json, tmx_image_layer& layer)
{
  json.at("image").get_to(layer.image);
  io::try_get_to(json, "transparentcolor", layer.transparent);
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_IMAGE_LAYER_HPP
