// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/runtime/plugin.hpp"
#include "tactile/tiled_tmj_format/api.hpp"
#include "tactile/tiled_tmj_format/tiled_tmj_format.hpp"

namespace tactile {

class TACTILE_TMJ_FORMAT_API TiledTmjFormatPlugin final : public IPlugin
{
 public:
  TACTILE_DELETE_COPY(TiledTmjFormatPlugin);
  TACTILE_DELETE_MOVE(TiledTmjFormatPlugin);

  TiledTmjFormatPlugin() = default;

  ~TiledTmjFormatPlugin() noexcept override = default;

  void load(IRuntime* runtime) override;

  void unload() override;

 private:
  IRuntime* mRuntime {};
  TiledTmjFormat mFormat {};
};

extern "C"
{
  TACTILE_TMJ_FORMAT_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_TMJ_FORMAT_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
