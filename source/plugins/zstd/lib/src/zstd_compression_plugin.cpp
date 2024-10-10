// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/zstd/zstd_compression_plugin.hpp"

#include <new>  // nothrow

#include "tactile/base/runtime/runtime.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

void ZstdCompressionPlugin::load(IRuntime* runtime)
{
  runtime::log(LogLevel::kTrace, "Loading Zstd compression plugin");
  mRuntime = runtime;

  mCompressor = std::make_unique<ZstdCompressionFormat>();
  mRuntime->set_compression_format(CompressionFormatId::kZstd, mCompressor.get());
}

void ZstdCompressionPlugin::unload()
{
  runtime::log(LogLevel::kTrace, "Unloading Zstd compression plugin");

  mRuntime->set_compression_format(CompressionFormatId::kZstd, nullptr);
  mRuntime = nullptr;

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
