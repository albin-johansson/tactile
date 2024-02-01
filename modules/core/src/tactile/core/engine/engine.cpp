// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/engine/engine.hpp"

#include <SDL2/SDL.h>
#include <imgui_impl_sdl2.h>

#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

Engine::Engine(IEngineApp* app, IRenderer* renderer)
  : mApp {require_not_null(app, "null app")},
    mRenderer {require_not_null(renderer, "null renderer")}
{}

void Engine::run()
{
  mApp->on_startup();

  bool running = true;
  while (running) {
    running = _poll_events();

    _check_framebuffer_scale();

    if (mRenderer->begin_frame().has_value()) {
      mApp->on_update();
      mRenderer->end_frame();
    }
  }

  mApp->on_shutdown();
}

auto Engine::_poll_events() -> bool
{
  bool keep_running = true;

  SDL_Event event {};
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);

    if (event.type == SDL_QUIT) {
      keep_running = false;
      break;
    }
  }

  return keep_running;
}

void Engine::_check_framebuffer_scale()
{
  const auto& io = ImGui::GetIO();
  const auto current_framebuffer_scale = io.DisplayFramebufferScale.x;

  if (mFramebufferScale != current_framebuffer_scale) {
    mFramebufferScale = current_framebuffer_scale;
    TACTILE_LOG_DEBUG("Framebuffer scale changed to {}", mFramebufferScale);

    mApp->on_framebuffer_scale_changed(mFramebufferScale);
  }
}

}  // namespace tactile::core
