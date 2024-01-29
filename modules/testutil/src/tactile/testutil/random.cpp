// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/testutil/random.hpp"

#include "tactile/foundation/math/rng.hpp"

namespace tactile::testutil {

auto make_random_byte_stream(const usize length) -> ByteStream
{
  ByteStream bytes {};
  bytes.reserve(length);

  for (usize i = 0; i < length; ++i) {
    bytes.push_back(static_cast<uint8>(next_random_u32(+'a', +'z')));
  }

  return bytes;
}

}  // namespace tactile::testutil
