// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use crate::core::Expected;

/// Represents supported compression modes.
#[repr(u8)]
#[derive(Copy, Clone, Debug, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub enum Compression {
  None,
  Zlib,
  Zstd,
}

/// Used to compress and/or decompress raw bytes.
pub trait Compressor {
  /// Compresses an arbitrary stream of bytes.
  fn compress(&self, data: &[u8]) -> Expected<Vec<u8>>;

  /// Decompresses a stream of compressed bytes.
  fn decompress(&self, data: &[u8]) -> Expected<Vec<u8>>;
}

mod texture;
pub use texture::TextureId;
