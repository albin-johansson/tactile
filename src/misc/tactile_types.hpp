#pragma once

#include <concepts>
#include <gsl/gsl>
#include <type_traits>

#include "nenya.hpp"

namespace tactile {
namespace detail {

// the tags are located here just to avoid auto-complete confusing tile_id and
// tile_id_t for example.

struct tile_id_t
{};

struct layer_id_t
{};

struct map_id_t
{};

struct tileset_id_t
{};

}  // namespace detail

using tile_id = nenya::mirror_type<int, detail::tile_id_t>;
using layer_id = nenya::mirror_type<int, detail::layer_id_t>;
using map_id = nenya::mirror_type<int, detail::map_id_t>;
using tileset_id = nenya::mirror_type<int, detail::tileset_id_t>;

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
