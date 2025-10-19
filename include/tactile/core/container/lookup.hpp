// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <stdexcept>
#include <utility>

#include "tactile/core/container/option.hpp"

namespace tactile {

/// Heuristic concept that indicates whether `T` has a map-like interface.
template <typename T>
concept MapLike = requires {
  typename T::key_type;
  typename T::mapped_type;
};

/// Returns a pointer to the value associated with a given key in a map, if any.
template <MapLike T, typename U>
auto try_get_from(T& map, const U& key) -> T::mapped_type*
{
  const auto iter = map.find(key);

  if (iter == map.end()) {
    return nullptr;
  }

  return &iter->second;
}

/// Returns a pointer to the value associated with a given key in a map, if any.
template <MapLike T, typename U>
auto try_get_from(const T& map, const U& key) -> const T::mapped_type*
{
  const auto iter = map.find(key);

  if (iter == map.end()) {
    return nullptr;
  }

  return &iter->second;
}

/// Returns the value associated with a given key in a map.
template <MapLike T, typename U>
auto get_from(T& map, const U& key) -> T::mapped_type&
{
  if (auto* const value = try_get_from(map, key)) {
    return *value;
  }

  throw std::invalid_argument {"[get_from]: invalid key"};
}

/// Returns the value associated with a given key in a map.
template <MapLike T, typename U>
auto get_from(const T& map, const U& key) -> const T::mapped_type&
{
  if (const auto* const value = try_get_from(map, key)) {
    return *value;
  }

  throw std::invalid_argument {"[get_from]: invalid key"};
}

/// Removes the value associated with a given key from a map, if any.
template <MapLike T, typename U>
auto extract_from(T& map, const U& key) -> Option<typename T::mapped_type>
{
  const auto iter = map.find(key);

  if (iter != map.end()) {
    auto value = std::move(iter->second);
    (void) map.erase(iter);
    return value;
  }

  return kNone;
}

}  // namespace tactile
