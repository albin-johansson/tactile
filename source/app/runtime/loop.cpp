// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "loop.hpp"

#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include "runtime/app_context.hpp"
#include "tactile/core/debug/exception.hpp"
#include "ui/imgui_context.hpp"

namespace tactile {

void EventLoop::start()
{
  mRunning = true;

  on_startup();

  const auto& io = ImGui::GetIO();

  ImVec2 prev_scale {};
  while (mRunning) {
    poll_events();

    // We reload the fonts when the framebuffer scale changes. Since it is initially zero,
    // we know that we will load the fonts at least once.
    if (const auto& scale = io.DisplayFramebufferScale; prev_scale.x != scale.x) {
      prev_scale = scale;
      ImGuiContext::reload_fonts();
    }

    on_pre_update();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    on_update();

    ImGui::Render();

    glViewport(0,
               0,
               static_cast<int>(io.DisplaySize.x),
               static_cast<int>(io.DisplaySize.y));
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    cen::gl::swap(get_window());
  }

  on_shutdown();
}

void EventLoop::stop()
{
  mRunning = false;
}

void EventLoop::poll_events()
{
  mKeyboard.refresh();

  cen::event_handler handler;
  while (handler.poll()) {
    ImGui_ImplSDL2_ProcessEvent(handler.data());

    switch (handler.type().value()) {
      case cen::event_type::quit:
        mRunning = false;
        break;

      default:
        on_event(handler);
        break;
    }
  }
}

}  // namespace tactile
