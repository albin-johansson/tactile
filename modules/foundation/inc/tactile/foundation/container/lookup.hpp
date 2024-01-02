// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // move

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/functional/maybe.hpp"

namespace tactile {

/**
 * Heuristic concept that indicates whether a type is "map-like".
 */
template <typename T>
concept MapLikeType = requires {
  typename T::key_type;
  typename T::mapped_type;
};

/**
 * Attempts to find an element in a map.
 *
 * \tparam T A map-like type.
 *
 * \param map The source map container.
 * \param key The key associated with the desired element.
 *
 * \return
 *    The found element; or a null pointer if none was found.
 */
template <MapLikeType T>
[[nodiscard]] auto find_in(T& map, const auto& key) -> typename T::mapped_type*
{
  const auto iter = map.find(key);
  return (iter != map.end()) ? &iter->second : nullptr;
}

/** \copydoc find_in() */
template <MapLikeType T>
[[nodiscard]] auto find_in(const T& map, const auto& key) -> const
    typename T::mapped_type*
{
  const auto iter = map.find(key);
  return (iter != map.end()) ? &iter->second : nullptr;
}

/**
 * Looks up an element in a map.
 *
 * \tparam T A map-like type.
 *
 * \param map The source map container.
 * \param key The key associated with the desired element.
 *
 * \return
 *    The found element.
 *
 * \throw Exception if no element is found.
 */
template <MapLikeType T>
[[nodiscard]] auto lookup_in(T& map, const auto& key) -> typename T::mapped_type&
{
  if (auto* elem = find_in(map, key)) {
    return *elem;
  }
  else {
    throw Exception {"Did not find key in map"};
  }
}

/** \copydoc lookup_in() */
template <MapLikeType T>
[[nodiscard]] auto lookup_in(const T& map, const auto& key) -> const
    typename T::mapped_type&
{
  if (const auto* elem = find_in(map, key)) {
    return *elem;
  }
  else {
    throw Exception {"Did not find key in map"};
  }
}

/**
 * Removes an element from a map.
 *
 * \tparam T A map-like type.
 *
 * \param map The source map container.
 * \param key The key associated with the element to remove.
 *
 * \return
 *    The removed element, or nothing if the element wasn't found.
 */
template <MapLikeType T>
auto erase_from(T& map, const auto& key) -> Maybe<typename T::mapped_type>
{
  if (const auto iter = map.find(key); iter != map.end()) {
    auto elem = std::move(iter->second);
    map.erase(iter);
    return elem;
  }

  return kNone;
}

/**
 * Indicates whether an element exists in a map.
 *
 * \tparam T A map-like type.
 *
 * \param map The source map container.
 * \param key The key associated with the element to look for.
 *
 * \return
 *    True if the element was found; false otherwise.
 */
template <MapLikeType T>
[[nodiscard]] auto exists_in(const T& map, const auto& key) -> bool
{
  return map.find(key) != map.end();
}

}  // namespace tactile