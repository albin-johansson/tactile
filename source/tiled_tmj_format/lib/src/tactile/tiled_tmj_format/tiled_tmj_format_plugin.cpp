// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tiled_tmj_format_plugin.hpp"

#include <new>  // nothrow

#include "tactile/runtime/runtime.hpp"

namespace tactile {

void TiledTmjFormatPlugin::load(Runtime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Loading Tiled TMJ format plugin");
  runtime.set_save_format(SaveFormatId::kTiledTmj, &mFormat);
}

void TiledTmjFormatPlugin::unload(Runtime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Unloading Tiled TMJ format plugin");
  runtime.set_save_format(SaveFormatId::kTiledTmj, nullptr);
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) TiledTmjFormatPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
