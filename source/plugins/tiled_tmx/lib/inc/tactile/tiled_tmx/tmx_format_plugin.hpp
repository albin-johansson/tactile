// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/runtime/plugin.hpp"
#include "tactile/tiled_tmx/api.hpp"

namespace tactile::tiled_tmx {

class TACTILE_TILED_TMX_API TmxFormatPlugin final : public IPlugin
{
 public:
  void load(IRuntime* runtime) override;

  void unload() override;

 private:
  IRuntime* m_runtime {};
  std::unique_ptr<ISaveFormat> m_format {};
};

extern "C"
{
  TACTILE_TILED_TMX_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_TILED_TMX_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile::tiled_tmx
