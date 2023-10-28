// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_plugin.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/render/render_context.hpp"

namespace tactile::gl {
namespace {

void _set_opengl_hints()
{
  if constexpr (kIsApple) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  }

  // OpenGL 4.1.0 core
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
}

}  // namespace

void OpenGLPlugin::on_load()
{
  TACTILE_LOG_DEBUG("Loading OpenGL renderer plugin...");

  if (SDL_WasInit(SDL_INIT_EVERYTHING) != SDL_INIT_EVERYTHING) {
    TACTILE_LOG_ERROR("OpenGL renderer plugin requires that SDL is initialized");
    return;
  }

  _set_opengl_hints();

  TACTILE_LOG_TRACE("Creating OpenGL window...");
  mWindow = make_unique<OpenGLWindow>();

  if (!gladLoadGLLoader(&SDL_GL_GetProcAddress)) {
    TACTILE_LOG_ERROR("Could not load OpenGL functions");
    return;
  }

  TACTILE_LOG_TRACE("Creating OpenGL renderer...");
  mRenderer = make_unique<OpenGLRenderer>(mWindow.get());

  TACTILE_LOG_DEBUG("GL_VERSION: {}",
                    reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  TACTILE_LOG_DEBUG("GL_RENDERER: {}",
                    reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

  RenderContext::get().set_window(mWindow.get());
  RenderContext::get().set_renderer(mRenderer.get());
}

void OpenGLPlugin::on_unload()
{
  TACTILE_LOG_DEBUG("Unloading OpenGL renderer plugin...");

  RenderContext::get().set_window(nullptr);
  RenderContext::get().set_renderer(nullptr);

  mRenderer.reset();
  mWindow.reset();
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
