// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <memory>  // unique_ptr

#include "tactile/base/prelude.hpp"
#include "tactile/base/runtime/plugin.hpp"
#include "tactile/zlib/api.hpp"
#include "tactile/zlib/zlib_compression_format.hpp"

namespace tactile {

/**
 * Manages the Zlib compression plugin.
 */
class TACTILE_ZLIB_API ZlibCompressionPlugin final : public IPlugin
{
 public:
  void load(IRuntime* runtime) override;

  void unload() override;

 private:
  IRuntime* mRuntime {};
  std::unique_ptr<ZlibCompressionFormat> mCompressor {};
};

extern "C"
{
  TACTILE_ZLIB_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_ZLIB_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
