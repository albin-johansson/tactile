// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/zlib_compression/zlib_compression_plugin.hpp"

#include <new>  // nothrow

#include "tactile/base/runtime.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

void ZlibCompressionPlugin::load(IRuntime& runtime)
{
  log(LogLevel::kTrace, "Loading Zlib compression plugin");

  mCompressor = std::make_unique<ZlibCompressor>();
  runtime.set_compression_format(CompressionFormat::kZlib, mCompressor.get());
}

void ZlibCompressionPlugin::unload(IRuntime& runtime)
{
  log(LogLevel::kTrace, "Unloading Zlib compression plugin");

  runtime.set_compression_format(CompressionFormat::kZlib, nullptr);
  mCompressor.reset();
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) ZlibCompressionPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
