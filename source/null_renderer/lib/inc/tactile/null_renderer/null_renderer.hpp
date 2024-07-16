// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/render/renderer.hpp"
#include "tactile/null_renderer/api.hpp"
#include "tactile/null_renderer/null_texture.hpp"

namespace tactile {

/**
 * A null renderer implementation.
 */
class TACTILE_NULL_RENDERER_API NullRenderer final : public IRenderer
{
 public:
  /**
   * Creates a renderer.
   *
   * \param window The associated window.
   */
  explicit NullRenderer(IWindow* window);

  [[nodiscard]]
  auto begin_frame() -> bool override;

  void end_frame() override;

  [[nodiscard]]
  auto load_texture(const char* image_path) -> Result<TextureID> override;

  void unload_texture(TextureID id) override;

  [[nodiscard]]
  auto find_texture(TextureID id) const -> const ITexture* override;

  void try_reload_fonts() override;

  [[nodiscard]]
  auto can_reload_fonts() const -> bool override;

  [[nodiscard]]
  auto get_window() -> IWindow* override;

  [[nodiscard]]
  auto get_window() const -> const IWindow* override;

 private:
  IWindow* mWindow;
  HashMap<TextureID, NullTexture> mTextures {};
  TextureID mNextTextureId {1};
};

}  // namespace tactile
