#pragma once

#include <NamedType/named_type.hpp>
#include <concepts>
#include <gsl/gsl>
#include <type_traits>

namespace tactile {
namespace detail {

template <typename Rep, typename Tag>
using id_type = fluent::NamedType<Rep,
                                  Tag,
                                  fluent::Comparable,
                                  fluent::Addable,
                                  fluent::Subtractable,
                                  fluent::PreIncrementable,
                                  fluent::PostIncrementable,
                                  fluent::Hashable,
                                  fluent::Printable>;

struct tile_id_t
{};

struct layer_id_t
{};

struct map_id_t
{};

struct tileset_id_t
{};

}  // namespace detail

using tile_id = detail::id_type<int, detail::tile_id_t>;
using layer_id = detail::id_type<int, detail::layer_id_t>;
using map_id = detail::id_type<int, detail::map_id_t>;
using tileset_id = detail::id_type<int, detail::tileset_id_t>;

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
