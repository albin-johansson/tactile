#pragma once

#include <concepts>
#include <gsl/gsl>
#include <type_traits>

#include "nenya.hpp"

namespace tactile {
namespace detail {

// the tags are located here just to avoid auto-complete confusing tile_id and
// tile_id_t for example.

struct tile_id_t final
{};

struct layer_id_t final
{};

struct map_id_t final
{};

struct tileset_id_t final
{};

struct tile_width_t final
{};

struct tile_height_t final
{};

}  // namespace detail

using tile_id = nenya::mirror_type<int, detail::tile_id_t>;
using layer_id = nenya::mirror_type<int, detail::layer_id_t>;
using map_id = nenya::mirror_type<int, detail::map_id_t>;
using tileset_id = nenya::mirror_type<int, detail::tileset_id_t>;
using tile_width = nenya::mirror_type<int, detail::tile_width_t>;
using tile_height = nenya::mirror_type<int, detail::tile_height_t>;

constexpr auto operator""_t(unsigned long long value) noexcept -> tile_id
{
  return tile_id{static_cast<int>(value)};
}

constexpr auto operator""_layer(unsigned long long value) noexcept -> layer_id
{
  return layer_id{static_cast<int>(value)};
}

constexpr auto operator""_map(unsigned long long value) noexcept -> map_id
{
  return map_id{static_cast<int>(value)};
}

constexpr auto operator""_ts(unsigned long long value) noexcept -> tileset_id
{
  return tileset_id{static_cast<int>(value)};
}

constexpr auto operator""_tw(unsigned long long value) noexcept -> tile_width
{
  return tile_width{static_cast<int>(value)};
}

constexpr auto operator""_th(unsigned long long value) noexcept -> tile_height
{
  return tile_height{static_cast<int>(value)};
}

inline constexpr tile_id empty{0};

// clang-format off

template <typename T> requires std::is_pointer_v<T>
using owner = gsl::owner<T>;

template <typename T> requires std::is_pointer_v<T>
using not_null = gsl::not_null<T>;

// clang-format on

using czstring = gsl::czstring<>;
using u16_czstring = gsl::cu16zstring<>;

}  // namespace tactile
