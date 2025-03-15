// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/basic/macros.hpp"
#include "tactile/core/basic/primitives.hpp"
#include "tactile/core/basic/result.hpp"
#include "tactile/core/basic/span.hpp"
#include "tactile/core/basic/vector.hpp"

namespace tactile {

/// Interface implemented by compression plugins.
class ICompressor
{
 public:
  TACTILE_INTERFACE_CLASS(ICompressor);

  /// Compresses a stream of bytes.
  virtual auto compress(Span<const u8> data) -> Result<Vector<u8>> = 0;

  /// Decompresses a stream of compressed bytes.
  virtual auto decompress(Span<const u8> data) -> Result<Vector<u8>> = 0;
};

}  // namespace tactile
