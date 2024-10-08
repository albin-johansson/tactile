// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <memory>  // unique_ptr

#include "tactile/base/prelude.hpp"
#include "tactile/runtime/plugin.hpp"
#include "tactile/zstd/api.hpp"
#include "tactile/zstd/zstd_compressor.hpp"

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
  std::unique_ptr<ZstdCompressor> mCompressor {};
};

extern "C"
{
  TACTILE_ZSTD_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_ZSTD_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
