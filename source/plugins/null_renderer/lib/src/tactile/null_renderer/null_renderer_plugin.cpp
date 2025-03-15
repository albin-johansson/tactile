// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/null_renderer/null_renderer_plugin.hpp"

#include <new>  // nothrow

#include "tactile/core/log/logger.hpp"
#include "tactile/core/plugin/runtime.hpp"
#include "tactile/null_renderer/null_renderer.hpp"

namespace tactile::null_renderer {

auto NullRendererPlugin::install(IRuntime& runtime) -> Result<void>
{
  TACTILE_LOG_DEBUG("Installing null renderer plugin");

  m_renderer = make_unique<NullRenderer>();
  runtime.set_renderer(m_renderer.get());

  return ok();
}

void NullRendererPlugin::uninstall(IRuntime& runtime)
{
  TACTILE_LOG_DEBUG("Uninstalling null renderer plugin");

  runtime.set_renderer(nullptr);
  m_renderer.reset();
}

auto tactile_create_plugin() -> IPlugin*
{
  return new (std::nothrow) NullRendererPlugin {};
}

void tactile_destroy_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile::null_renderer
