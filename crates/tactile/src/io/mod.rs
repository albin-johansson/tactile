// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use crate::core::Expected;

/// Used to compress and/or decompress raw bytes.
pub trait Compressor {
  /// Compresses an arbitrary stream of bytes.
  fn compress(&self, data: &[u8]) -> Expected<Vec<u8>>;

  /// Decompresses a stream of compressed bytes.
  fn decompress(&self, data: &[u8]) -> Expected<Vec<u8>>;
}
