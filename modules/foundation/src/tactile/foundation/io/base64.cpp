// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/base64.hpp"

#include <cppcodec/base64_rfc4648.hpp>

namespace tactile {

auto Base64::encode(const ByteSpan bytes) -> String
{
  return cppcodec::base64_rfc4648::encode(bytes.data(), bytes.size_bytes());
}

auto Base64::decode(const StringView data) -> ByteStream
{
  return cppcodec::base64_rfc4648::decode(data);
}

}  // namespace tactile
