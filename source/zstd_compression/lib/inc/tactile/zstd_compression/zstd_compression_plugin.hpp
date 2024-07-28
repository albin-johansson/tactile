// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/runtime/plugin.hpp"
#include "tactile/zstd_compression/api.hpp"
#include "tactile/zstd_compression/zstd_compressor.hpp"

namespace tactile {

/**
 * Manages the Zstd compression plugin.
 */
class TACTILE_ZSTD_API ZstdCompressionPlugin final : public IPlugin
{
 public:
  void load(IRuntime* runtime) override;

  void unload() override;

 private:
  IRuntime* mRuntime {};
  Unique<ZstdCompressor> mCompressor {};
};

extern "C"
{
  TACTILE_ZSTD_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_ZSTD_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
