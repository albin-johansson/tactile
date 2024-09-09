// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/null_renderer/null_renderer.hpp"

#include <utility>  // move

namespace tactile {

NullRenderer::NullRenderer(IWindow* window)
  : mWindow {window}
{}

auto NullRenderer::begin_frame() -> bool
{
  return true;
}

void NullRenderer::end_frame()
{}

auto NullRenderer::load_texture(const std::filesystem::path& image_path)
    -> std::expected<TextureID, std::error_code>
{
  auto texture = NullTexture::load(image_path);

  if (!texture.has_value()) {
    return std::unexpected {texture.error()};
  }

  const auto texture_id = mNextTextureId;
  ++mNextTextureId.value;

  mTextures.insert_or_assign(texture_id, std::move(*texture));

  return texture_id;
}

void NullRenderer::unload_texture(const TextureID id)
{
  mTextures.erase(id);
}

auto NullRenderer::find_texture(const TextureID id) const -> const ITexture*
{
  const auto iter = mTextures.find(id);

  if (iter != mTextures.end()) {
    return &iter->second;
  }

  return nullptr;
}

void NullRenderer::try_reload_fonts()
{}

auto NullRenderer::can_reload_fonts() const -> bool
{
  return false;
}

auto NullRenderer::get_window() -> IWindow*
{
  return mWindow;
}

auto NullRenderer::get_window() const -> const IWindow*
{
  return mWindow;
}

auto NullRenderer::get_imgui_context() -> ImGuiContext*
{
  return nullptr;
}

void NullRenderer::process_event(const SDL_Event&)
{}

}  // namespace tactile
