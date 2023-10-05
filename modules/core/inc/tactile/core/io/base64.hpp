// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/span.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/io/byte_stream.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

class Base64 final {
 public:
  TACTILE_STATIC_CLASS(Base64);

  [[nodiscard]]
  TACTILE_CORE_API static auto encode(ByteSpan bytes) -> String;

  [[nodiscard]]
  TACTILE_CORE_API static auto decode(StringView data) -> ByteStream;
};

}  // namespace tactile
