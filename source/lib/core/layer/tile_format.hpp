// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

namespace tactile {

/// Represents the different supported tile layer data encodings.
enum class TileEncoding {
  Plain,  ///< Encode the tile layer data in ordinary plain text.
  Base64  ///< Use Base64 encoding.
};

/// Represents the different supported tile data compression methods.
enum class TileCompression {
  None,  ///< Apply no compression to tile layer data.
  Zlib,  ///< Use the Zlib compression library.
  Zstd   ///< Use the zstd compression library.
};

/// Describes the tile layer data format used by a map.
///
/// An instance of this class is guaranteed to be in a valid state,
/// and will as such raise exceptions if you try to specify an invalid
/// tile format.
class TileFormat final {
 public:
  void set_encoding(TileEncoding encoding);

  void set_compression(TileCompression compression);

  void set_zlib_compression_level(int level);

  void set_zstd_compression_level(int level);

  [[nodiscard]] auto encoding() const -> TileEncoding;

  [[nodiscard]] auto compression() const -> TileCompression;

  [[nodiscard]] auto zlib_compression_level() const -> int;

  [[nodiscard]] auto zstd_compression_level() const -> int;

  /// Does the current encoding support tile compression?
  [[nodiscard]] auto supports_any_compression() const -> bool;

  [[nodiscard]] auto can_use_compression_strategy(TileCompression compression) const
      -> bool;

  [[nodiscard]] static auto is_valid_zlib_compression_level(int level) -> bool;

  [[nodiscard]] static auto is_valid_zstd_compression_level(int level) -> bool;

  [[nodiscard]] static auto min_zlib_compression_level() -> int;
  [[nodiscard]] static auto max_zlib_compression_level() -> int;

  [[nodiscard]] static auto min_zstd_compression_level() -> int;
  [[nodiscard]] static auto max_zstd_compression_level() -> int;

 private:
  TileEncoding mEncoding {TileEncoding::Plain};
  TileCompression mCompression {TileCompression::None};
  int mZlibCompressionLevel {-1};
  int mZstdCompressionLevel {3};
};

}  // namespace tactile
