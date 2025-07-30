// Copyright (C) 2025 Albin Johansson

#include <gmock/gmock.h>
#include <gtest/gtest.h>

import tactile.core.prelude;
import tactile.core.base64;

namespace tactile::tests {
namespace {

TEST(Base64, EncodeDecode)
{
  // See https://en.m.wikipedia.org/wiki/Base64#Examples
  const Vector<u8> data {
    'M', 'a', 'n', 'y', ' ', 'h', 'a', 'n', 'd', 's', ' ', 'm', 'a', 'k',
    'e', ' ', 'l', 'i', 'g', 'h', 't', ' ', 'w', 'o', 'r', 'k', '.',
  };

  String encoded_data {};
  base64_encode(data, encoded_data);

  ASSERT_EQ(encoded_data, "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu");

  Vector<u8> decoded_data {};
  base64_decode(encoded_data, decoded_data);

  EXPECT_THAT(decoded_data, testing::ContainerEq(data));
}

}  // namespace
}  // namespace tactile::tests
