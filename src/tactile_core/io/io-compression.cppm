// Copyright (C) 2025 Albin Johansson

export module tactile.io:compression;

export import tactile.common;

export namespace tactile {

/// Represents supported compression kinds.
enum class CompressionKind : u8
{
  kZlib,
  kZstd,
};

/// Interface for compression providers.
class ICompressor
{
 protected:
  ICompressor() = default;

  ICompressor(ICompressor&&) noexcept = default;

  ICompressor(const ICompressor&) = default;

  auto operator=(ICompressor&&) noexcept -> ICompressor& = default;

  auto operator=(const ICompressor&) -> ICompressor& = default;

 public:
  virtual ~ICompressor() noexcept = default;

  /// Compresses a stream of bytes.
  virtual auto compress(Span<const u8> data) -> Result<Vector<u8>> = 0;

  /// Decompresses a stream of compressed bytes.
  virtual auto decompress(Span<const u8> data) -> Result<Vector<u8>> = 0;
};

}  // namespace tactile
