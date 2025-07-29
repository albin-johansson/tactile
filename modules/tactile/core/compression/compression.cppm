// Copyright (C) 2025 Albin Johansson

export module tactile.core.compression;

import tactile.core.prelude;
import tactile.core.error;

export namespace tactile {

/// Represents supported compression algorithms.
enum class CompressionAlgorithm : u8
{
  kZlib,
  kZstd,
};

/// Interface for compression format implementations.
class ICompressionFormat
{
 protected:
  ICompressionFormat() = default;

  ICompressionFormat(ICompressionFormat&&) noexcept = default;

  ICompressionFormat(const ICompressionFormat&) = default;

  auto operator=(ICompressionFormat&&) noexcept
      -> ICompressionFormat& = default;

  auto operator=(const ICompressionFormat&) -> ICompressionFormat& = default;

 public:
  virtual ~ICompressionFormat() noexcept = default;

  /// Compresses a stream of bytes.
  [[nodiscard]]
  virtual auto compress(Span<const u8> data) const -> Result<Vector<u8>> = 0;

  /// Decompresses a stream of compressed bytes.
  [[nodiscard]]
  virtual auto decompress(Span<const u8> data) const -> Result<Vector<u8>> = 0;
};

/// A thin wrapper over a collection of compression formats.
class Compressor final
{
 public:
  /// Compresses a stream of bytes using a given algorithm.
  [[nodiscard]]
  auto compress_with(CompressionAlgorithm algorithm, Span<const u8> data) const
      -> Result<Vector<u8>>;

  /// Decompresses a stream of bytes using a given algorithm.
  [[nodiscard]]
  auto decompress_with(CompressionAlgorithm algorithm,
                       Span<const u8> data) const -> Result<Vector<u8>>;

  /// Sets the compression format implementation for a given algorithm.
  void set_format(CompressionAlgorithm algorithm,
                  Unique<ICompressionFormat> format);

 private:
  HashMap<CompressionAlgorithm, Unique<ICompressionFormat>> m_formats {};
};

}  // namespace tactile
