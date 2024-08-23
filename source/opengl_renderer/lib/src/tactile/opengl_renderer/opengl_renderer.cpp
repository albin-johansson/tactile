// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl_renderer/opengl_renderer.hpp"

#include <cstdlib>        // malloc, free
#include <list>           // list
#include <memory>         // nothrow
#include <optional>       // optional
#include <unordered_map>  // unordered_map
#include <utility>        // move

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/render/window.hpp"
#include "tactile/opengl_renderer/opengl_error.hpp"
#include "tactile/opengl_renderer/opengl_imgui.hpp"
#include "tactile/opengl_renderer/opengl_texture.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

struct GLContextDeleter final
{
  void operator()(void* context) noexcept
  {
    SDL_GL_DeleteContext(context);
  }
};

struct OpenGLRenderer::Data final  // NOLINT(*-member-init)
{
  ImGuiContext* context;
  IWindow* window;
  Unique<void, GLContextDeleter> gl_context;
  std::optional<GLImGuiBackendWrapper> imgui_backend;
  std::optional<GLImGuiRendererWrapper> imgui_renderer;
  std::unordered_map<TextureID, OpenGLTexture> textures;
  TextureID next_texture_id;
};

auto OpenGLRenderer::make(IWindow* window, ImGuiContext* context)
    -> std::expected<OpenGLRenderer, std::error_code>
{
  if (SDL_WasInit(SDL_INIT_VIDEO) != SDL_INIT_VIDEO) {
    return std::unexpected {make_error(OpenGLError::kNotReady)};
  }

  if (window == nullptr || context == nullptr) {
    return std::unexpected {make_error(OpenGLError::kInvalidParam)};
  }

  OpenGLRenderer renderer {};
  renderer.mData->window = window;
  renderer.mData->context = context;
  renderer.mData->next_texture_id = TextureID {1};

  renderer.mData->gl_context.reset(SDL_GL_CreateContext(window->get_handle()));
  if (!renderer.mData->gl_context) {
    return std::unexpected {make_error(OpenGLError::kContextError)};
  }

  // NOLINTBEGIN(*-no-malloc)
  ImGui::SetAllocatorFunctions([](const usize size, void*) { return std::malloc(size); },
                               [](void* ptr, void*) { std::free(ptr); });
  // NOLINTEND(*-no-malloc)
  ImGui::SetCurrentContext(context);

  if (!gladLoadGLLoader(&SDL_GL_GetProcAddress)) {
    return std::unexpected {make_error(OpenGLError::kLoaderError)};
  }

  if (auto imgui_backend = GLImGuiBackendWrapper::init(window->get_handle(), context)) {
    renderer.mData->imgui_backend.emplace(std::move(*imgui_backend));
  }
  else {
    return std::unexpected {imgui_backend.error()};
  }

  if (auto imgui_renderer = GLImGuiRendererWrapper::init()) {
    renderer.mData->imgui_renderer.emplace(std::move(*imgui_renderer));
  }
  else {
    return std::unexpected {imgui_renderer.error()};
  }

  SDL_GL_SetSwapInterval(1);

  return renderer;
}

OpenGLRenderer::OpenGLRenderer() :
  mData {std::make_unique<Data>()}
{}

OpenGLRenderer::OpenGLRenderer(OpenGLRenderer&& other) noexcept = default;

auto OpenGLRenderer::operator=(OpenGLRenderer&& other) noexcept -> OpenGLRenderer& = default;

OpenGLRenderer::~OpenGLRenderer() noexcept = default;

auto OpenGLRenderer::begin_frame() -> bool
{
  ImGui_ImplSDL2_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  const auto& io = ImGui::GetIO();
  glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return true;
}

void OpenGLRenderer::end_frame()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if constexpr (kOnMacos) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }
  SDL_GL_SwapWindow(mData->window->get_handle());
}

auto OpenGLRenderer::load_texture(const char* image_path)
    -> std::expected<TextureID, std::error_code>
{
  if (image_path == nullptr) {
    return std::unexpected {make_error(OpenGLError::kInvalidParam)};
  }

  auto texture = OpenGLTexture::load(image_path);
  if (!texture.has_value()) {
    return std::unexpected {texture.error()};
  }

  auto& data = *mData;

  const auto texture_id = data.next_texture_id;
  ++data.next_texture_id.value;

  mData->textures.try_emplace(texture_id, std::move(*texture));
  return texture_id;
}

void OpenGLRenderer::unload_texture(const TextureID id)
{
  mData->textures.erase(id);
}

auto OpenGLRenderer::find_texture(const TextureID id) const -> const ITexture*
{
  if (const auto iter = mData->textures.find(id); iter != mData->textures.end()) {
    return &iter->second;
  }

  return nullptr;
}

void OpenGLRenderer::try_reload_fonts()
{
  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_CreateFontsTexture();
}

auto OpenGLRenderer::can_reload_fonts() const -> bool
{
  return true;
}

auto OpenGLRenderer::get_window() -> IWindow*
{
  return mData->window;
}

auto OpenGLRenderer::get_window() const -> const IWindow*
{
  return mData->window;
}

}  // namespace tactile
