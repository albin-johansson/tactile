// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_renderer.hpp"

#include <cstdlib>  // malloc, free
#include <utility>  // exchange

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::gl {

OpenGLRenderer::OpenGLRenderer(OpenGLWindow* window, ImGuiContext* imgui_context)
  : mWindow {window},
    mImGuiContext {imgui_context},
    mNextTextureId {1},
    mPrimed {true}
{}

auto OpenGLRenderer::create(OpenGLWindow* window) -> Result<OpenGLRenderer>
{
  (void) require_not_null(window, "invalid null window");

  auto* imgui_context = ImGui::CreateContext();
  if (!imgui_context) {
    TACTILE_LOG_ERROR("Could not create ImGui context");
    return unexpected(make_generic_error(GenericError::kInitFailure));
  }

  ImGui::SetAllocatorFunctions([](const usize size, void*) { return std::malloc(size); },
                               [](void* ptr, void*) { std::free(ptr); });

  if (!ImGui_ImplSDL2_InitForOpenGL(window->get_handle(), imgui_context)) {
    TACTILE_LOG_ERROR("Could not initialize SDL2 ImGui backend");
    return unexpected(make_generic_error(GenericError::kInitFailure));
  }

  if (!ImGui_ImplOpenGL3_Init("#version 410 core")) {
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(imgui_context);

    TACTILE_LOG_ERROR("Could not initialize OpenGL ImGui backend");
    return unexpected(make_generic_error(GenericError::kInitFailure));
  }

  return OpenGLRenderer {window, imgui_context};
}

OpenGLRenderer::OpenGLRenderer(OpenGLRenderer&& other) noexcept
  : mWindow {std::exchange(other.mWindow, nullptr)},
    mImGuiContext {std::exchange(other.mImGuiContext, nullptr)},
    mTextures {std::exchange(other.mTextures, TextureMap {})},
    mNextTextureId {std::exchange(other.mNextTextureId, TextureID {})},
    mPrimed {std::exchange(other.mPrimed, false)}
{}

auto OpenGLRenderer::operator=(OpenGLRenderer&& other) noexcept -> OpenGLRenderer&
{
  if (this != &other) {
    mWindow = std::exchange(other.mWindow, nullptr);
    mImGuiContext = std::exchange(other.mImGuiContext, nullptr);
    mTextures = std::exchange(other.mTextures, TextureMap {});
    mNextTextureId = std::exchange(other.mNextTextureId, TextureID {});
    mPrimed = std::exchange(other.mPrimed, false);
  }

  return *this;
}

OpenGLRenderer::~OpenGLRenderer() noexcept
{
  if (mPrimed) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(mImGuiContext);
  }
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

}  // namespace tactile::gl
