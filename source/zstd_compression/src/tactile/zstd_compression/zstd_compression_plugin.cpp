// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/zstd_compression/zstd_compression_plugin.hpp"

#include <new>  // nothrow

#include "tactile/runtime/runtime.hpp"

namespace tactile {

void ZstdCompressionPlugin::load(Runtime& runtime)
{
  mCompressor = std::make_unique<ZstdCompressor>();
  runtime.set_compression_provider(CompressionType::kZstd, mCompressor.get());
}

void ZstdCompressionPlugin::unload(Runtime& runtime)
{
  runtime.set_compression_provider(CompressionType::kZstd, nullptr);
  mCompressor.reset();
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) ZstdCompressionPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
