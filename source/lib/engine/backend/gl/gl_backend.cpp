/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "gl_backend.hpp"

#include <GL/glew.h>
#include <centurion/opengl.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"

namespace tactile {

OpenGLBackend::OpenGLBackend(SDL_Window* window, SDL_GLContext context)
    : mWindow {window}
{
  spdlog::debug("Initializing OpenGL backend");

  if (!ImGui_ImplSDL2_InitForOpenGL(window, context)) {
    throw TactileError {"Could not initialize ImGui SDL2 backend"};
  }

  if (!ImGui_ImplOpenGL3_Init()) {
    ImGui_ImplSDL2_Shutdown();
    throw TactileError {"Could not initialize ImGui OpenGL backend"};
  }
}

OpenGLBackend::~OpenGLBackend() noexcept
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
}

void OpenGLBackend::process_event(const SDL_Event* event)
{
  ImGui_ImplSDL2_ProcessEvent(event);
}

auto OpenGLBackend::new_frame() -> Result
{
  ImGui_ImplSDL2_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  const auto& io = ImGui::GetIO();
  glViewport(0,
             0,
             static_cast<int>(io.DisplaySize.x),
             static_cast<int>(io.DisplaySize.y));

  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return success;
}

void OpenGLBackend::end_frame()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(mWindow);
}

auto OpenGLBackend::reload_font_resources() -> Result
{
  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_CreateFontsTexture();
  return success;
}

}  // namespace tactile
