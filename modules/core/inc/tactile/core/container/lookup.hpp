// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/debug/error.hpp"
#include "tactile/core/type/maybe.hpp"

namespace tactile {

template <typename T>
[[nodiscard]] auto find_in(T& map, const auto& key) -> typename T::mapped_type*
{
  const auto iter = map.find(key);
  return (iter != map.end()) ? &iter->second : nullptr;
}

template <typename T>
[[nodiscard]] auto find_in(const T& map, const auto& key) -> const
    typename T::mapped_type*
{
  const auto iter = map.find(key);
  return (iter != map.end()) ? &iter->second : nullptr;
}

template <typename T>
[[nodiscard]] auto lookup_in(T& map, const auto& key) -> typename T::mapped_type&
{
  if (auto* elem = find_in(map, key)) {
    return *elem;
  }
  else {
    throw Error {"Did not find key in map"};
  }
}

template <typename T>
[[nodiscard]] auto lookup_in(const T& map, const auto& key) -> const
    typename T::mapped_type&
{
  if (const auto* elem = find_in(map, key)) {
    return *elem;
  }
  else {
    throw Error {"Did not find key in map"};
  }
}

template <typename T>
auto erase_from(T& map, const auto& key) -> Maybe<typename T::mapped_type>
{
  if (const auto iter = map.find(key); iter != map.end()) {
    auto elem = iter->second;
    map.erase(iter);
    return elem;
  }

  return kNone;
}

template <typename T>
[[nodiscard]] auto exists_in(const T& container, const auto& key) -> bool
{
  return container.find(key) != container.end();
}

}  // namespace tactile