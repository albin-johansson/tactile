/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "loop.hpp"

#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include "cfg/configuration.hpp"
#include "cfg/fonts.hpp"
#include "misc/panic.hpp"

namespace tactile {

AEventLoop::AEventLoop(AppConfiguration* cfg) : mCfg{cfg}
{
  if (!mCfg) {
    panic("Invalid null application configuration!");
  }
}

void AEventLoop::start()
{
  mRunning = true;

  on_startup();

  const auto& io = ImGui::GetIO();
  auto& window = mCfg->window();

  ImVec2 prevScale{};
  while (mRunning) {
    poll_events();

    /* We reload the fonts when the framebuffer scale changes. Since it is initially zero,
       we know that we will load the fonts at least once. */
    if (const auto& scale = io.DisplayFramebufferScale; prevScale.x != scale.x) {
      prevScale = scale;
      reload_fonts();
    }

    on_pre_update();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    on_update();

    ImGui::EndFrame();
    ImGui::Render();

    glViewport(0,
               0,
               static_cast<int>(io.DisplaySize.x),
               static_cast<int>(io.DisplaySize.y));
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    cen::gl::swap(window);
  }

  on_shutdown();
}

void AEventLoop::stop()
{
  mRunning = false;
}

void AEventLoop::poll_events()
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