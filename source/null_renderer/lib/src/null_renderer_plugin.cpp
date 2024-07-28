// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/null_renderer/null_renderer_plugin.hpp"

#include <new>  // nothrow

#include "tactile/base/runtime.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

void NullRendererPlugin::load(IRuntime* runtime)
{
  log(LogLevel::kTrace, "Loading null renderer plugin");
  mRuntime = runtime;

  mRuntime->init_window(0);
  auto* window = mRuntime->get_window();

  if (!window) {
    log(LogLevel::kError, "Could not initialize window");
    return;
  }

  mRenderer = std::make_unique<NullRenderer>(window);
  mRuntime->set_renderer(mRenderer.get());
}

void NullRendererPlugin::unload()
{
  log(LogLevel::kTrace, "Unloading null renderer plugin");

  mRuntime->set_renderer(nullptr);
  mRuntime = nullptr;

  mRenderer.reset();
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) NullRendererPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
