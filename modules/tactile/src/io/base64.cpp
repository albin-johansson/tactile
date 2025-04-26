// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module;

#include <cppcodec/base64_default_rfc4648.hpp>

module tactile.io;

import tactile.core;

namespace tactile {

void base64_encode(const Span<const u8> data, String& encoded_data)
{
  base64::encode(encoded_data, data);
}

void base64_decode(const StringView data, Vector<u8>& decoded_data)
{
  base64::decode(decoded_data, data);
}

}  // namespace tactile
