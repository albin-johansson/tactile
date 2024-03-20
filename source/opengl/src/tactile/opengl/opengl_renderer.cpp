// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl/opengl_renderer.hpp"

#include <list>     // list
#include <utility>  // move

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>

#include "tactile/base/container/maybe.hpp"
#include "tactile/opengl/opengl_error.hpp"
#include "tactile/opengl/opengl_imgui_context.hpp"
#include "tactile/opengl/opengl_imgui_impl_opengl3.hpp"
#include "tactile/opengl/opengl_imgui_impl_sdl2.hpp"
#include "tactile/opengl/opengl_texture.hpp"
#include "tactile/opengl/opengl_window.hpp"

namespace tactile {
namespace {

void _prepare_sdl_opengl_hints()
{
  if constexpr (kOnMacos) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
}

}  // namespace

struct OpenGLRenderer::Data final {
  Maybe<OpenGLWindow> window;
  Maybe<OpenGLImGuiContext> imgui_context;
  Maybe<OpenGLImGuiImplSDL2> imgui_sdl2;
  Maybe<OpenGLImGuiImplOpenGL3> imgui_opengl3;
  std::list<OpenGLTexture> textures {};  // NB: allocated textures must be stable.
};

auto OpenGLRenderer::make() -> Result<OpenGLRenderer>
{
  if (SDL_WasInit(SDL_INIT_VIDEO) != SDL_INIT_VIDEO) {
    return unexpected(make_error(OpenGLError::kNotReady));
  }

  _prepare_sdl_opengl_hints();

  OpenGLRenderer renderer {};

  if (auto window = OpenGLWindow::make()) {
    renderer.mData->window.emplace(std::move(*window));
  }
  else {
    return propagate_unexpected(window);
  }

  if (!gladLoadGLLoader(&SDL_GL_GetProcAddress)) {
    return unexpected(make_error(OpenGLError::kLoaderError));
  }

  if (auto imgui_context = OpenGLImGuiContext::make()) {
    renderer.mData->imgui_context.emplace(std::move(*imgui_context));
  }
  else {
    return propagate_unexpected(imgui_context);
  }

  if (auto impl_sdl2 = OpenGLImGuiImplSDL2::init(*renderer.mData->window)) {
    renderer.mData->imgui_sdl2.emplace(std::move(*impl_sdl2));
  }
  else {
    return propagate_unexpected(impl_sdl2);
  }

  if (auto impl_opengl3 = OpenGLImGuiImplOpenGL3::init()) {
    renderer.mData->imgui_opengl3.emplace(std::move(*impl_opengl3));
  }
  else {
    return propagate_unexpected(impl_opengl3);
  }

  return renderer;
}

OpenGLRenderer::OpenGLRenderer()
  : mData {std::make_unique<Data>()}
{}

OpenGLRenderer::OpenGLRenderer(OpenGLRenderer&& other) noexcept = default;

auto OpenGLRenderer::operator=(OpenGLRenderer&& other) noexcept
    -> OpenGLRenderer& = default;

OpenGLRenderer::~OpenGLRenderer() noexcept = default;

auto OpenGLRenderer::begin_frame() -> bool
{
  mData->imgui_sdl2->begin_frame();
  mData->imgui_opengl3->begin_frame();
  ImGui::NewFrame();

  const auto& io = ImGui::GetIO();
  glViewport(0,
             0,
             static_cast<int>(io.DisplaySize.x),
             static_cast<int>(io.DisplaySize.y));

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return true;
}

void OpenGLRenderer::end_frame()
{
  ImGui::Render();
  mData->imgui_opengl3->render(ImGui::GetDrawData());
  mData->window->swap_buffers();
}

auto OpenGLRenderer::load_texture(const char* image_path) -> ITexture*
{
  if (image_path == nullptr) {
    return nullptr;
  }

  if (auto texture = OpenGLTexture::load(image_path)) {
    auto& texture_ref = mData->textures.emplace_back(std::move(*texture));
    return &texture_ref;
  }

  return nullptr;
}

void OpenGLRenderer::try_reload_fonts()
{
  mData->imgui_opengl3->reload_fonts_texture();
}

auto OpenGLRenderer::can_reload_fonts() const -> bool
{
  return true;
}

auto OpenGLRenderer::get_window() -> IWindow*
{
  return &mData->window.value();
}

auto OpenGLRenderer::get_window() const -> const IWindow*
{
  return &mData->window.value();
}

auto OpenGLRenderer::get_imgui_context() -> ImGuiContext*
{
  return mData->imgui_context->get();
}

}  // namespace tactile
