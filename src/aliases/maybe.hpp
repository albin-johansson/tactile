#pragma once

#include <optional>  // optional

namespace Tactile {

template <typename T>
using Maybe = std::optional<T>;

inline constexpr std::nullopt_t nothing = std::nullopt;

}  // namespace Tactile
