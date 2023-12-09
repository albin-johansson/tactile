// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/span.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/byte_stream.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class TACTILE_FOUNDATION_API Base64 final {
 public:
  TACTILE_STATIC_CLASS(Base64);

  [[nodiscard]]
  static auto encode(ByteSpan bytes) -> String;

  [[nodiscard]]
  static auto decode(StringView data) -> ByteStream;
};

}  // namespace tactile
