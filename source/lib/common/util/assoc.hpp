// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // equality_comparable_with
#include <utility>   // forward

#include "tactile/core/debug/exception.hpp"

namespace tactile {

template <typename M, std::equality_comparable_with<typename M::key_type> K>
[[nodiscard]] auto lookup_in(M& map, K&& key) -> typename M::mapped_type&
{
  const auto iter = map.find(std::forward<K>(key));
  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw Exception {"Invalid key!"};
  }
}

template <typename M, std::equality_comparable_with<typename M::key_type> K>
[[nodiscard]] auto lookup_in(const M& map, K&& key) -> const typename M::mapped_type&
{
  const auto iter = map.find(std::forward<K>(key));
  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw Exception {"Invalid key!"};
  }
}

template <typename M, std::equality_comparable_with<typename M::key_type> K>
[[nodiscard]] auto has_key(const M& map, K&& key) -> bool
{
  return map.find(std::forward<K>(key)) != map.end();
}

}  // namespace tactile
