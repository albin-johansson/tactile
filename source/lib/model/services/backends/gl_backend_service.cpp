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

#include "gl_backend_service.hpp"

#include <bit>  // bit_cast

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include "io/texture_loader.hpp"
#include "model/textures/texture_components.hpp"
#include "tactile/core/common/error.hpp"
#include "tactile/core/common/prelude.hpp"

namespace tactile {

GLBackendService::GLBackendService(SDL_Window* window, SDL_GLContext context)
    : mWindow {window}
{
  if (!ImGui_ImplSDL2_InitForOpenGL(mWindow, context)) {
    throw Error {"Could not initialize ImGui SDL2 backend"};
  }

  if (!ImGui_ImplOpenGL3_Init()) {
    ImGui_ImplSDL2_Shutdown();
    throw Error {"Could not initialize ImGui OpenGL backend"};
  }
}

GLBackendService::~GLBackendService() noexcept
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
}

void GLBackendService::process_event(const SDL_Event& event)
{
  ImGui_ImplSDL2_ProcessEvent(&event);
}

auto GLBackendService::new_frame() -> Result
{
  ImGui_ImplSDL2_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  const auto& io = ImGui::GetIO();
  glViewport(0,
             0,
             static_cast<int>(io.DisplaySize.x),
             static_cast<int>(io.DisplaySize.y));

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return success;
}

void GLBackendService::end_frame()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(mWindow);
}

void GLBackendService::reload_font_texture()
{
  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_CreateFontsTexture();
}

void GLBackendService::prepare_texture(Registry& registry,
                                       const Entity texture_entity,
                                       const TextureData& texture_data)
{
  uint texture_id = 0;
  glGenTextures(1, &texture_id);

  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               texture_data.size.x,
               texture_data.size.y,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               texture_data.pixels.get());

  auto& gl_texture = registry.add<OpenGLTexture>(texture_entity);
  gl_texture.id = texture_id;

  auto& texture = registry.get<Texture>(texture_entity);
  texture.handle = std::bit_cast<void*>(static_cast<uintptr>(gl_texture.id));
}

void GLBackendService::destroy_texture(void* handle)
{
  const auto texture_id = static_cast<uint>(std::bit_cast<uintptr>(handle));
  glDeleteTextures(1, &texture_id);
}

}  // namespace tactile