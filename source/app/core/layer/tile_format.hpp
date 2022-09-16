/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
