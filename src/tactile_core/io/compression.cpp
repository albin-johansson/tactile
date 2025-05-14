// Copyright (C) 2025 Albin Johansson

module tactile.core.io;

import std;
import tactile.core.log;

namespace tactile {

auto Compressor::compress_with(const CompressionAlgorithm algorithm,
                               const Span<const u8> data) const -> Result<Vector<u8>>
{
  const auto iter = m_formats.find(algorithm);

  if (iter == m_formats.end()) {
    get_logger().log(LogLevel::kError,
                     "Tried to compress data with unsupported algorithm");
    return err(Error::kInvalidOp);
  }

  return iter->second->compress(data);
}

auto Compressor::decompress_with(const CompressionAlgorithm algorithm,
                                 const Span<const u8> data) const
    -> Result<Vector<u8>>
{
  const auto iter = m_formats.find(algorithm);

  if (iter == m_formats.end()) {
    get_logger().log(LogLevel::kError,
                     "Tried to decompress data with unsupported algorithm");
    return err(Error::kInvalidOp);
  }

  return iter->second->decompress(data);
}

void Compressor::set_format(const CompressionAlgorithm algorithm,
                            Unique<ICompressionFormat> format)
{
  m_formats.insert_or_assign(algorithm, std::move(format));
}

}  // namespace tactile
