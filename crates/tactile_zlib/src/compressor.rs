// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use flate2::Compression;
use flate2::write::ZlibDecoder;
use flate2::write::ZlibEncoder;
use std::io::Write;
use tactile::core::Errc;
use tactile::core::Expected;
use tactile::io::Compressor;

/// A compressor that uses the [zlib][1] format.
///
/// [1]: https://www.zlib.net
#[derive(Copy, Clone, Debug, Default)]
pub struct ZlibCompressor {}

impl ZlibCompressor {
  /// Creates a zlib compressor.
  pub fn new() -> Self {
    Self {}
  }
}

impl Compressor for ZlibCompressor {
  fn compress(&self, data: &[u8]) -> Expected<Vec<u8>> {
    let mut encoder = ZlibEncoder::new(Vec::new(), Compression::default());
    encoder
      .write_all(data)
      .and_then(|_| encoder.finish())
      .inspect_err(|err| eprintln!("Could not compress data with Zlib: {:?}", err))
      .map_err(|_| Errc::CouldNotCompress)
  }

  fn decompress(&self, data: &[u8]) -> Expected<Vec<u8>> {
    let mut decoder = ZlibDecoder::new(Vec::new());
    decoder
      .write_all(data)
      .and_then(|_| decoder.finish())
      .inspect_err(|err| eprintln!("Could not decompress data with Zlib: {:?}", err))
      .map_err(|_| Errc::CouldNotDecompress)
  }
}
