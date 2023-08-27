// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>     // hash, equal_to
#include <type_traits>    // true_type
#include <unordered_map>  // unordered_map

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/string.hpp"

namespace tactile {

struct TACTILE_CORE_API StringHash final {
  using hash_type = std::hash<StringView>;
  using is_transparent = std::true_type;

  [[nodiscard]] auto operator()(const char* str) const -> usize
  {
    return hash_type {}(str);
  }

  [[nodiscard]] auto operator()(StringView str) const -> usize
  {
    return hash_type {}(str);
  }

  [[nodiscard]] auto operator()(const String& str) const -> usize
  {
    return hash_type {}(str);
  }
};

/** Variant of hash map optimized for string keys, using heterogeneous lookups. */
template <typename T>
using StringMap = std::unordered_map<String, T, StringHash, std::equal_to<>>;

}  // namespace tactile