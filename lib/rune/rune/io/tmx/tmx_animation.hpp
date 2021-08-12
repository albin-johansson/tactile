#ifndef RUNE_TMX_ANIMATION_HPP
#define RUNE_TMX_ANIMATION_HPP

#include <chrono>  // milliseconds
#include <vector>  // vector

#include "../../aliases/json_type.hpp"
#include "../json_utils.hpp"
#include "tmx_local_id.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_frame final
{
  tmx_local_id tile{};
  std::chrono::milliseconds duration{};
};

struct tmx_animation final
{
  std::vector<tmx_frame> frames;
};

inline void from_json(const json_type& json, tmx_frame& frame)
{
  using ms_t = std::chrono::milliseconds;

  io::emplace_to(json, "tileid", frame.tile);
  frame.duration = ms_t{json.at("duration").get<ms_t::rep>()};
}

inline void from_json(const json_type& json, tmx_animation& animation)
{
  io::get_to(json, animation.frames);
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_ANIMATION_HPP
