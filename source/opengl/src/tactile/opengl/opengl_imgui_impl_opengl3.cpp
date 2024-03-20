// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_imgui_impl_opengl3.hpp"

#include <utility>  // exchange

#include <imgui_impl_opengl3.h>

#include "tactile/opengl/opengl_error.hpp"

namespace tactile {

auto OpenGLImGuiImplOpenGL3::init() -> Result<OpenGLImGuiImplOpenGL3>
{
  if (!ImGui_ImplOpenGL3_Init("#version 410 core")) {
    return unexpected(make_error(OpenGLError::kImGuiError));
  }

  return OpenGLImGuiImplOpenGL3 {};
}

OpenGLImGuiImplOpenGL3::OpenGLImGuiImplOpenGL3(OpenGLImGuiImplOpenGL3&& other) noexcept
  : mActive {std::exchange(other.mActive, false)}
{}

OpenGLImGuiImplOpenGL3::~OpenGLImGuiImplOpenGL3() noexcept
{
  if (mActive) {
    ImGui_ImplOpenGL3_Shutdown();
    mActive = false;
  }
}

// NOLINTNEXTLINE(*-convert-member-functions-to-static)
void OpenGLImGuiImplOpenGL3::begin_frame()
{
  ImGui_ImplOpenGL3_NewFrame();
}

// NOLINTNEXTLINE(*-convert-member-functions-to-static)
void OpenGLImGuiImplOpenGL3::render(ImDrawData* draw_data)
{
  ImGui_ImplOpenGL3_RenderDrawData(draw_data);
}

// NOLINTNEXTLINE(*-convert-member-functions-to-static)
void OpenGLImGuiImplOpenGL3::reload_fonts_texture()
{
  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_CreateFontsTexture();
}

}  // namespace tactile
