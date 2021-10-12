#pragma once

#include <expected.hpp>  // expected

namespace Tactile {

template <typename T, typename E>
using Expected = tl::expected<T, E>;

}  // namespace Tactile