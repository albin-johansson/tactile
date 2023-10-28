// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_plugin.hpp"

namespace tactile::gl {

void OpenGLPlugin::on_load()
{
  TACTILE_LOG_DEBUG("Loading OpenGL renderer plugin...");
}

void OpenGLPlugin::on_unload()
{
  TACTILE_LOG_DEBUG("Unloading OpenGL renderer plugin...");
}

auto tactile_create_plugin() -> IPlugin*
{
  return new OpenGLPlugin {};
}

void tactile_destroy_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile::gl
