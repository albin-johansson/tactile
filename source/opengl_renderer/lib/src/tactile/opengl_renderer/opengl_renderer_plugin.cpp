// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl_renderer/opengl_renderer_plugin.hpp"

#include <new>  // nothrow

#include <SDL2/SDL.h>
#include <imgui.h>

#include "tactile/base/runtime.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/runtime/runtime.hpp"

namespace tactile {
namespace opengl_renderer_plugin {

void set_hints()
{
  if constexpr (kOnMacos) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
}

}  // namespace opengl_renderer_plugin

void OpenGLRendererPlugin::load(IRuntime* runtime)
{
  m_runtime = runtime;
  opengl_renderer_plugin::set_hints();

  m_runtime->init_window(SDL_WINDOW_OPENGL);
  auto* window = m_runtime->get_window();

  if (!window) {
    log(LogLevel::kError, "Could not initialize OpenGL window");
    return;
  }

  ImGui::SetAllocatorFunctions([](const usize size, void*) { return runtime_malloc(size); },
                               [](void* ptr, void*) { runtime_free(ptr); });

  if (auto renderer = OpenGLRenderer::make(window)) {
    m_renderer = std::move(*renderer);
    m_runtime->set_renderer(&m_renderer.value());
  }
}

void OpenGLRendererPlugin::unload()
{
  m_runtime->set_renderer(nullptr);
  m_runtime = nullptr;
  m_renderer.reset();
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
