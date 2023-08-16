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

#include "tactile/opengl-rhi/gl_render_device.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include "tactile/core/common/error.hpp"

namespace tactile {

void GLOpenGLContextDeleter::operator()(void* context) noexcept
{
  SDL_GL_DeleteContext(context);
}

void GLImGuiContextDeleter::operator()(ImGuiContext* context) noexcept
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext(context);
}

void GLRenderDevice::initialize(Window* window)
{
  mWindow = window;
  if (!mWindow) {
    throw Error {"Null window provided to OpenGL render device"};
  }

  auto* window_handle = reinterpret_cast<SDL_Window*>(mWindow->get_handle());

  mOpenGLContext.reset(SDL_GL_CreateContext(window_handle));
  if (!mOpenGLContext) {
    throw Error {"Could not create OpenGL context"};
  }

  if (!gladLoadGLLoader(&SDL_GL_GetProcAddress)) {
    throw Error {"Could not load OpenGL functions"};
  }

  mImGuiContext.reset(ImGui::CreateContext());
  if (!mImGuiContext) {
    throw Error {"Could not create ImGui context"};
  }

  if (!ImGui_ImplSDL2_InitForOpenGL(window_handle, SDL_GL_GetCurrentContext())) {
    throw Error {"Could not initialize ImGui SDL2 backend"};
  }

  if (!ImGui_ImplOpenGL3_Init("#version 410 core")) {
    throw Error {"Could not initialize ImGui OpenGL backend"};
  }
}

void GLRenderDevice::process_imgui_event(const SDL_Event& event)
{
  ImGui_ImplSDL2_ProcessEvent(&event);
}

auto GLRenderDevice::begin_frame() -> bool
{
  const auto width = mWindow->get_width();
  const auto height = mWindow->get_height();
  glViewport(0, 0, width, height);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplSDL2_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  return true;
}

void GLRenderDevice::end_frame()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
}

auto GLRenderDevice::get_imgui_context() -> ImGuiContext*
{
  return mImGuiContext.get();
}

}  // namespace tactile
