// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

export module tactile.io:compression;

import tactile.core;

export namespace tactile {

/// Represents supported compression kinds.
enum class CompressionKind
{
  kZlib,
  kZstd,
};

/// Interface for compression providers.
class ICompressor
{
 public:
  virtual ~ICompressor() noexcept = default;

  /// Compresses a stream of bytes.
  virtual auto compress(Span<const u8> data) -> Result<Vector<u8>> = 0;

  /// Decompresses a stream of compressed bytes.
  virtual auto decompress(Span<const u8> data) -> Result<Vector<u8>> = 0;
};

}  // namespace tactile
