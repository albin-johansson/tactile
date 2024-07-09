// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl_renderer/opengl_renderer_plugin.hpp"

#include <new>  // nothrow

#include <SDL2/SDL.h>

#include "tactile/runtime/runtime.hpp"

namespace tactile {

void OpenGLRendererPlugin::load(Runtime& runtime)
{
  if constexpr (kOnMacos) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

  runtime.init_window(SDL_WINDOW_OPENGL);  // TODO error handling

  auto* window = runtime.get_window();
  auto* imgui_context = runtime.get_imgui_context();

  if (auto renderer = OpenGLRenderer::make(window, imgui_context)) {
    mRenderer = std::move(*renderer);
    runtime.set_renderer(&mRenderer.value());
  }
}

void OpenGLRendererPlugin::unload(Runtime& runtime)
{
  runtime.set_renderer(nullptr);
  mRenderer.reset();
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) OpenGLRendererPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
