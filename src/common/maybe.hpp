#pragma once

#include <optional.hpp>  // optional

namespace Tactile {

template <typename T>
using Maybe = tl::optional<T>;

constexpr tl::nullopt_t nothing = tl::nullopt;

}  // namespace Tactile
