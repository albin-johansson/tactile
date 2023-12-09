// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_renderer.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::gl {

OpenGLRenderer::OpenGLRenderer(OpenGLWindow* window)
  : mWindow {require_not_null(window, "invalid null window")},
    mImGuiContext {ImGui::CreateContext()}
{
  if (!ImGui_ImplSDL2_InitForOpenGL(mWindow->get_handle(), mImGuiContext)) {
    TACTILE_LOG_FATAL("Could not initialize SDL2 ImGui backend");
    throw Exception {"Could not initialize SDL2 ImGui backend"};
  }

  if (!ImGui_ImplOpenGL3_Init("#version 410 core")) {
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(mImGuiContext);

    TACTILE_LOG_FATAL("Could not initialize OpenGL ImGui backend");
    throw Exception {"Could not initialize OpenGL ImGui backend"};
  }
}

OpenGLRenderer::~OpenGLRenderer() noexcept
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext(mImGuiContext);
}

auto OpenGLRenderer::begin_frame() -> Result<void>
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

  return kSuccess;
}

void OpenGLRenderer::end_frame()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  mWindow->swap_framebuffer();
}

void OpenGLRenderer::reload_fonts_texture()
{
  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_CreateFontsTexture();
}

auto OpenGLRenderer::can_reload_fonts_texture() const -> bool
{
  return true;
}

auto OpenGLRenderer::load_texture(const FilePath& image_path) -> OpenGLTexture*
{
  if (auto texture = OpenGLTexture::load(image_path)) {
    auto [iter, _] =
        mTextures.try_emplace(mNextTextureId,
                              make_unique<OpenGLTexture>(std::move(*texture)));
    mNextTextureId = TextureID {mNextTextureId.value + 1};

    return iter->second.get();
  }

  return nullptr;
}

auto OpenGLRenderer::get_window() -> OpenGLWindow*
{
  return mWindow;
}

auto OpenGLRenderer::get_window() const -> const OpenGLWindow*
{
  return mWindow;
}

auto OpenGLRenderer::get_imgui_context() -> ImGuiContext*
{
  return mImGuiContext;
}

auto OpenGLRenderer::get_imgui_allocator_functions() const -> ImGuiAllocatorFunctions
{
  ImGuiAllocatorFunctions functions {};

  void* user_data = nullptr;
  ImGui::GetAllocatorFunctions(&functions.alloc_fn, &functions.free_fn, &user_data);

  return functions;
}

}  // namespace tactile::gl
