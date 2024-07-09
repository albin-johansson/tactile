// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/zlib_compression/zlib_compression_plugin.hpp"

#include <new>  // nothrow

#include "tactile/runtime/runtime.hpp"

namespace tactile {

void ZlibCompressionPlugin::load(Runtime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Loading Zlib compression plugin");

  mCompressor = std::make_unique<ZlibCompressor>();
  runtime.set_compression_provider(CompressionType::kZlib, mCompressor.get());
}

void ZlibCompressionPlugin::unload(Runtime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Unloading Zlib compression plugin");

  runtime.set_compression_provider(CompressionType::kZlib, nullptr);
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
