#ifndef RUNE_TMX_DATA_HPP
#define RUNE_TMX_DATA_HPP

#include <cassert>  // assert
#include <string>   // string
#include <variant>  // variant
#include <vector>   // vector

#include "../../aliases/integers.hpp"
#include "../../aliases/json_type.hpp"
#include "tmx_global_id.hpp"

namespace rune {

struct tmx_data final
{
  using gid_data = std::vector<tmx_global_id>;
  using base64_data = std::string;
  using data_type = std::variant<gid_data, base64_data>;

  data_type tile_data;
};

inline void from_json(const json_type& json, tmx_data& data)
{
  assert(json.is_array() || json.is_string());

  if (json.is_array())
  {
    auto& gidData = data.tile_data.emplace<tmx_data::gid_data>();
    for (const auto& [key, value] : json.items())
    {
      gidData.emplace_back(value.get<uint>());
    }
  }
  else if (json.is_string())
  {
    data.tile_data.emplace<tmx_data::base64_data>(json.get<tmx_data::base64_data>());
  }
}

}  // namespace rune

#endif  // RUNE_TMX_DATA_HPP
