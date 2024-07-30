// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_plugin.hpp"

#include <new>  // nothrow

#include "tactile/base/runtime.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

void TmjFormatPlugin::load(IRuntime* runtime)
{
  log(LogLevel::kTrace, "Loading Tiled TMJ format plugin");
  mRuntime = runtime;

  mFormat = std::make_unique<TmjSaveFormat>(mRuntime);
  mRuntime->set_save_format(SaveFormatId::kTiledTmj, mFormat.get());
}

void TmjFormatPlugin::unload()
{
  log(LogLevel::kTrace, "Unloading Tiled TMJ format plugin");

  mRuntime->set_save_format(SaveFormatId::kTiledTmj, nullptr);
  mRuntime = nullptr;

  mFormat.reset();
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) TmjFormatPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
