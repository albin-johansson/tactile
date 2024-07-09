// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl_renderer/opengl_imgui.hpp"

#include <utility>  // exchange

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include "tactile/opengl_renderer/opengl_error.hpp"

namespace tactile {

auto GLImGuiBackendWrapper::init(SDL_Window* window, ImGuiContext* context)
    -> Result<GLImGuiBackendWrapper>
{
  if (!ImGui_ImplSDL2_InitForOpenGL(window, context)) {
    return unexpected(make_error(OpenGLError::kImGuiError));
  }

  return GLImGuiBackendWrapper {};
}

GLImGuiBackendWrapper::GLImGuiBackendWrapper() noexcept
  : mActive {true}
{}

GLImGuiBackendWrapper::~GLImGuiBackendWrapper() noexcept
{
  if (mActive) {
    ImGui_ImplSDL2_Shutdown();
    mActive = false;
  }
}

GLImGuiBackendWrapper::GLImGuiBackendWrapper(
    GLImGuiBackendWrapper&& other) noexcept
  : mActive {std::exchange(other.mActive, false)}
{}

auto GLImGuiRendererWrapper::init() -> Result<GLImGuiRendererWrapper>
{
  if (!ImGui_ImplOpenGL3_Init("#version 410 core")) {
    return unexpected(make_error(OpenGLError::kImGuiError));
  }

  return GLImGuiRendererWrapper {};
}

GLImGuiRendererWrapper::GLImGuiRendererWrapper() noexcept
  : mActive {true}
{}

GLImGuiRendererWrapper::~GLImGuiRendererWrapper() noexcept
{
  if (mActive) {
    ImGui_ImplOpenGL3_Shutdown();
    mActive = false;
  }
}

GLImGuiRendererWrapper::GLImGuiRendererWrapper(
    GLImGuiRendererWrapper&& other) noexcept
  : mActive {std::exchange(other.mActive, false)}
{}

}  // namespace tactile
