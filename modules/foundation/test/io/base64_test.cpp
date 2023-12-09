// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/base64.hpp"

#include <gtest/gtest.h>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/math/rng.hpp"

using namespace tactile;

/// \tests tactile::Base64::encode
/// \tests tactile::Base64::decode
TEST(Base64, EncodeAndDecodeKnownExample)
{
  // See https://en.m.wikipedia.org/wiki/Base64#Examples
  const String initial_data = "Many hands make light work.";

  const auto encoded_data = Base64::encode(make_byte_span(initial_data));
  EXPECT_EQ(encoded_data, "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu");

  const auto decoded_data = Base64::decode(encoded_data);
  const Vector<uint8> expected_decoded_data = {'M', 'a', 'n', 'y', ' ',       //
                                               'h', 'a', 'n', 'd', 's', ' ',  //
                                               'm', 'a', 'k', 'e', ' ',       //
                                               'l', 'i', 'g', 'h', 't', ' ',  //
                                               'w', 'o', 'r', 'k', '.'};

  EXPECT_EQ(decoded_data, expected_decoded_data);
}

/// \tests tactile::Base64::encode
/// \tests tactile::Base64::decode
TEST(Base64, EncodeAndDecodeRandomData)
{
  ByteStream initial_data;
  initial_data.reserve(512);

  for (usize i = 0; i < 512; ++i) {
    initial_data.push_back(static_cast<uint8>(next_random_i32(+'a', +'z')));
  }

  const auto encoded_data = Base64::encode(make_byte_span(initial_data));
  const auto decoded_data = Base64::decode(encoded_data);

  EXPECT_EQ(decoded_data, initial_data);
}
