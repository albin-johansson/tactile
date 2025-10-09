// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <functional>
#include <unordered_map>

#include "tactile/core/container/string.hpp"
#include "tactile/core/primitives.hpp"

namespace tactile {

/// The hasher type used by StringMap.
class StringHasher final
{
 public:
  using is_transparent [[maybe_unused]] = void;
  using hash_type = std::hash<StringView>;

  auto operator()(const char* str) const -> usize
  {
    return hash_type {}(str);
  }

  auto operator()(const StringView str) const -> usize
  {
    return hash_type {}(str);
  }

  auto operator()(const String& str) const -> usize
  {
    return hash_type {}(str);
  }
};

/// A hash map optimized for string keys (supports heterogeneous lookups).
template <typename T>
using StringMap = std::unordered_map<String, T, StringHasher, std::equal_to<>>;

}  // namespace tactile
