// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/null_renderer/null_renderer_plugin.hpp"

#include <new>  // nothrow

#include "tactile/runtime/runtime.hpp"

namespace tactile {

void NullRendererPlugin::load(IRuntime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Loading null renderer plugin");

  runtime.init_window(0);
  auto* window = runtime.get_window();

  if (!window) {
    Runtime::log(LogLevel::kError, "Could not initialize window");
    return;
  }

  mRenderer = std::make_unique<NullRenderer>(window);
  runtime.set_renderer(mRenderer.get());
}

void NullRendererPlugin::unload(IRuntime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Unloading null renderer plugin");

  runtime.set_renderer(nullptr);
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
