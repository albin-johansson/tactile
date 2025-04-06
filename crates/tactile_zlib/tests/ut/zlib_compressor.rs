// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

use googletest::prelude::anything;
use googletest::prelude::assert_that;
use googletest::prelude::eq;
use googletest::prelude::ok;
use tactile::io::Compressor;
use tactile_zlib::ZlibCompressor;

#[test]
fn compress_decompress() {
  let compressor = ZlibCompressor::new();
  let raw_data = b"Lorem ipsum dolor sit amet, consectetur adipiscing elit.";

  let compressed_data = compressor.compress(raw_data);
  assert_that!(compressed_data, ok(anything()));

  let decompressed_data = compressor.decompress(compressed_data.unwrap().as_slice());
  assert_that!(decompressed_data, ok(eq(raw_data)));
}
