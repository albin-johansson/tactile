// Copyright (C) 2025 Albin Johansson

export module tactile.io:base64;

export import tactile.common;

export namespace tactile {

/// Encodes raw bytes as a Base64 string.
void base64_encode(Span<const u8> data, String& encoded_data);

/// Decodes a Base64 string to raw bytes.
void base64_decode(StringView data, Vector<u8>& decoded_data);

}  // namespace tactile
