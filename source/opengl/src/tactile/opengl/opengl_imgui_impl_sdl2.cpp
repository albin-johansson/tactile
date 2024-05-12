// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_imgui_impl_sdl2.hpp"

#include <utility>  // exchange

#include <imgui_impl_sdl2.h>

#include "tactile/opengl/opengl_error.hpp"
#include "tactile/render/window.hpp"

namespace tactile {

auto OpenGLImGuiImplSDL2::init(IWindow& window, ImGuiContext* context)
    -> Result<OpenGLImGuiImplSDL2>
{
  if (!ImGui_ImplSDL2_InitForOpenGL(window.get_handle(), context)) {
    return unexpected(make_error(OpenGLError::kImGuiError));
  }

  return OpenGLImGuiImplSDL2 {};
}

OpenGLImGuiImplSDL2::OpenGLImGuiImplSDL2(OpenGLImGuiImplSDL2&& other) noexcept
  : mActive {std::exchange(other.mActive, false)}
{}

OpenGLImGuiImplSDL2::~OpenGLImGuiImplSDL2() noexcept
{
  if (mActive) {
    ImGui_ImplSDL2_Shutdown();
    mActive = false;
  }
}

// NOLINTNEXTLINE(*-convert-member-functions-to-static)
void OpenGLImGuiImplSDL2::begin_frame()
{
  ImGui_ImplSDL2_NewFrame();
}

}  // namespace tactile
