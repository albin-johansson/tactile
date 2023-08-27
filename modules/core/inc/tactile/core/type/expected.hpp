// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <type_traits>  // decay_t
#include <utility>      // forward

#include <tl/expected.hpp>

namespace tactile {

template <typename T, typename E>
using Expected = tl::expected<T, E>;

template <typename E>
using Unexpected = tl::unexpected<E>;

template <typename E>
[[nodiscard]] auto unexpected(E&& e) -> Unexpected<std::decay_t<E>>
{
  return tl::make_unexpected(std::forward<E>(e));
}

template <typename T, typename E>
[[nodiscard]] auto propagate_unexpected(const Expected<T, E>& e) -> Unexpected<E>
{
  return unexpected(e.error());
}

}  // namespace tactile
