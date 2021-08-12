#ifndef RUNE_TMX_LAYER_HPP
#define RUNE_TMX_LAYER_HPP

#include <cassert>   // assert
#include <concepts>  // same_as
#include <memory>    // unique_ptr, make_unique
#include <string>    // string
#include <variant>   // variant, monostate
#include <vector>    // vector

#include "../../aliases/integers.hpp"
#include "../../aliases/json_type.hpp"
#include "../../aliases/maybe.hpp"
#include "../json_utils.hpp"
#include "tmx_color.hpp"
#include "tmx_image_layer.hpp"
#include "tmx_layer_type.hpp"
#include "tmx_object.hpp"
#include "tmx_object_layer.hpp"
#include "tmx_property.hpp"
#include "tmx_tile_layer.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

struct tmx_layer;

struct tmx_group final
{
  std::vector<std::unique_ptr<tmx_layer>> layers;
};

struct tmx_layer final
{
  using data_type = std::variant<std::monostate,
                                 tmx_tile_layer,
                                 tmx_image_layer,
                                 tmx_object_layer,
                                 tmx_group>;

  tmx_layer_type type{tmx_layer_type::tile_layer};
  int id{};
  int width{};
  int height{};
  int start_x{};
  int start_y{};
  float parallax_x{1};
  float parallax_y{1};
  float offset_x{};
  float offset_y{};
  float opacity{1};

  maybe<tmx_color> tint;
  tmx_properties properties;

  data_type data;

  std::string name;
  bool visible{true};
};

using tmx_layers = std::vector<tmx_layer>;

void from_json(const json_type& json, tmx_layer& layer);

inline void from_json(const json_type& json, tmx_group& group)
{
  const auto& layers = json.at("layers");
  group.layers.reserve(layers.size());
  for (const auto& [key, value] : layers.items())
  {
    group.layers.push_back(std::make_unique<tmx_layer>(value.get<tmx_layer>()));
  }
}

inline void from_json(const json_type& json, tmx_layer& layer)
{
  json.at("type").get_to(layer.type);

  io::try_get_to(json, "name", layer.name);
  io::try_get_to(json, "opacity", layer.opacity);
  io::try_get_to(json, "visible", layer.visible);
  io::try_get_to(json, "id", layer.id);
  io::try_get_to(json, "width", layer.width);
  io::try_get_to(json, "height", layer.height);
  io::try_get_to(json, "startx", layer.start_x);
  io::try_get_to(json, "starty", layer.start_y);
  io::try_get_to(json, "parallaxx", layer.parallax_x);
  io::try_get_to(json, "parallaxy", layer.parallax_y);
  io::try_get_to(json, "offsetx", layer.offset_x);
  io::try_get_to(json, "offsety", layer.offset_y);
  io::try_get_to(json, "tintcolor", layer.tint);

  io::try_get_to(json, "properties", layer.properties);

  switch (layer.type)
  {
    default:
      assert(false && "from_json() for tmx_layer is missing branch!");

    case tmx_layer_type::tile_layer:
      layer.data = json.get<tmx_tile_layer>();
      break;

    case tmx_layer_type::object_layer:
      layer.data = json.get<tmx_object_layer>();
      break;

    case tmx_layer_type::image_layer:
      layer.data = json.get<tmx_image_layer>();
      break;

    case tmx_layer_type::group:
      layer.data = json.get<tmx_group>();
      break;
  }
}

/// \} End of group tmx

namespace tmx {

/// \addtogroup tmx
/// \{

// clang-format off

template <typename T>
concept layer_value_type = std::same_as<T, tmx_tile_layer> ||
                           std::same_as<T, tmx_object_layer> ||
                           std::same_as<T, tmx_image_layer> ||
                           std::same_as<T, tmx_group>;

// clang-format on

/// \name Layer functions
/// \{

template <layer_value_type T>
[[nodiscard]] auto get(const tmx_layer& layer) -> const T&
{
  return std::get<T>(layer.data);
}

[[nodiscard]] inline auto get_tile_layer(const tmx_layer& layer) -> const tmx_tile_layer&
{
  return get<tmx_tile_layer>(layer);
}

[[nodiscard]] inline auto get_image_layer(const tmx_layer& layer)
    -> const tmx_image_layer&
{
  return get<tmx_image_layer>(layer);
}

[[nodiscard]] inline auto get_object_layer(const tmx_layer& layer)
    -> const tmx_object_layer&
{
  return get<tmx_object_layer>(layer);
}

[[nodiscard]] inline auto get_group(const tmx_layer& layer) -> const tmx_group&
{
  return get<tmx_group>(layer);
}

template <layer_value_type T>
[[nodiscard]] auto try_get(const tmx_layer& layer) noexcept -> const T*
{
  return std::get_if<T>(&layer.data);
}

[[nodiscard]] inline auto try_get_tile_layer(const tmx_layer& layer) noexcept
    -> const tmx_tile_layer*
{
  return try_get<tmx_tile_layer>(layer);
}

[[nodiscard]] inline auto try_get_object_layer(const tmx_layer& layer) noexcept
    -> const tmx_object_layer*
{
  return try_get<tmx_object_layer>(layer);
}

[[nodiscard]] inline auto try_get_image_layer(const tmx_layer& layer) noexcept
    -> const tmx_image_layer*
{
  return try_get<tmx_image_layer>(layer);
}

[[nodiscard]] inline auto try_get_group(const tmx_layer& layer) noexcept
    -> const tmx_group*
{
  return try_get<tmx_group>(layer);
}

template <layer_value_type T>
[[nodiscard]] auto is(const tmx_layer& layer) noexcept -> bool
{
  return std::holds_alternative<T>(layer.data);
}

[[nodiscard]] inline auto is_tile_layer(const tmx_layer& layer) noexcept -> bool
{
  return is<tmx_tile_layer>(layer);
}

[[nodiscard]] inline auto is_object_layer(const tmx_layer& layer) noexcept -> bool
{
  return is<tmx_object_layer>(layer);
}

[[nodiscard]] inline auto is_image_layer(const tmx_layer& layer) noexcept -> bool
{
  return is<tmx_image_layer>(layer);
}

[[nodiscard]] inline auto is_group(const tmx_layer& layer) noexcept -> bool
{
  return is<tmx_group>(layer);
}

/// \} End of layer functions

/// \} End of group tmx

}  // namespace tmx
}  // namespace rune

#endif  // RUNE_TMX_LAYER_HPP
