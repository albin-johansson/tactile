// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

[[nodiscard]]
TACTILE_FOUNDATION_API auto deserialize_int2(StringView str) -> Maybe<Int2>;

[[nodiscard]]
TACTILE_FOUNDATION_API auto deserialize_int3(StringView str) -> Maybe<Int3>;

[[nodiscard]]
TACTILE_FOUNDATION_API auto deserialize_int4(StringView str) -> Maybe<Int4>;

[[nodiscard]]
TACTILE_FOUNDATION_API auto deserialize_float2(StringView str) -> Maybe<Float2>;

[[nodiscard]]
TACTILE_FOUNDATION_API auto deserialize_float3(StringView str) -> Maybe<Float3>;

[[nodiscard]]
TACTILE_FOUNDATION_API auto deserialize_float4(StringView str) -> Maybe<Float4>;

}  // namespace tactile
