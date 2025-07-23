// Copyright (C) 2025 Albin Johansson

module tactile.core.io;

import tactile.core.ext.cppcodec;

namespace tactile {

void base64_encode(const Span<const u8> data, String& encoded_data)
{
  cppcodec::base64_rfc4648::encode(encoded_data, data);
}

void base64_decode(const StringView data, Vector<u8>& decoded_data)
{
  cppcodec::base64_rfc4648::decode(decoded_data, data);
}

}  // namespace tactile
