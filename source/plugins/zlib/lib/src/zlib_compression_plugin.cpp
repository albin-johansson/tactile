// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/zlib/zlib_compression_plugin.hpp"

#include <new>  // nothrow

#include "tactile/base/runtime/runtime.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

void ZlibCompressionPlugin::load(IRuntime* runtime)
{
  runtime::log(LogLevel::kTrace, "Loading Zlib compression plugin");
  mRuntime = runtime;

  mCompressor = std::make_unique<ZlibCompressor>();
  mRuntime->set_compression_format(CompressionFormat::kZlib, mCompressor.get());
}

void ZlibCompressionPlugin::unload()
{
  runtime::log(LogLevel::kTrace, "Unloading Zlib compression plugin");

  mRuntime->set_compression_format(CompressionFormat::kZlib, nullptr);
  mRuntime = nullptr;

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
