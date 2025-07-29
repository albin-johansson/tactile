// Copyright (C) 2025 Albin Johansson

/// Provides a compression format implementation using Zlib.
export module tactile.zlib;

import tactile.core;

export namespace tactile {

/// Provides compression using the Zlib library.
///
/// See https://github.com/madler/zlib.
class ZlibCompressionFormat final : public ICompressionFormat
{
 public:
  [[nodiscard]]
  auto compress(Span<const u8> data) const -> Result<Vector<u8>> override;

  [[nodiscard]]
  auto decompress(Span<const u8> data) const -> Result<Vector<u8>> override;
};

}  // namespace tactile
