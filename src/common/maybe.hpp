#pragma once

#include <optional>  // optional

namespace Tactile {

template <typename T>
using Maybe = std::optional<T>;

constexpr std::nullopt_t nothing = std::nullopt;

}  // namespace Tactile
