// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <filesystem>    // path
#include <system_error>  // error_code

#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/render/renderer_options.hpp"

struct ImGuiContext;
union SDL_Event;

namespace tactile {

class IWindow;
class ITexture;

/**
 * Provides the high level renderer backend API.
 */
class IRenderer
{
 public:
  TACTILE_INTERFACE_CLASS(IRenderer);

  /**
   * Attempts to begin a new render frame.
   *
   * \return
   * True if successful; false if something went wrong.
   */
  [[nodiscard]]
  virtual auto begin_frame() -> bool = 0;

  /**
   * Ends the current render frame.
   *
   * \pre The \c begin_frame function must've been called before this function.
   */
  virtual void end_frame() = 0;

  /**
   * Tries to load a texture from disk.
   *
   * \param image_path The path to the image.
   *
   * \return
   * The identifier assigned to the loaded texture.
   */
  [[nodiscard]]
  virtual auto load_texture(const std::filesystem::path& image_path)
      -> std::expected<TextureID, std::error_code> = 0;

  /**
   * Unloads a previously loaded texture.
   *
   * \param id The identifier of the texture to remove.
   */
  virtual void unload_texture(TextureID id) = 0;

  /**
   * Attempts to find the texture associated with a given identifier.
   *
   * \param id The identifier of the desired texture.
   *
   * \return
   * A pointer to the texture if found; a null pointer otherwise.
   */
  [[nodiscard]]
  virtual auto find_texture(TextureID id) const -> const ITexture* = 0;

  /**
   * Reloads the fonts texture, if possible.
   */
  virtual void try_reload_fonts() = 0;

  /**
   * Indicates whether the renderer supports reloading the fonts texture.
   *
   * \return
   * True if the fonts texture can be reloaded; false otherwise.
   */
  [[nodiscard]]
  virtual auto can_reload_fonts() const -> bool = 0;

  /**
   * Returns the associated window.
   *
   * \return
   * A window.
   */
  [[nodiscard]]
  virtual auto get_window() -> IWindow* = 0;

  /**
   * \copydoc get_window()
   */
  [[nodiscard]]
  virtual auto get_window() const -> const IWindow* = 0;

  /**
   * Returns the associated Dear ImGui context.
   *
   * \return
   * A Dear ImGui context.
   */
  [[nodiscard]]
  virtual auto get_imgui_context() -> ImGuiContext* = 0;

  /**
   * Processes a single event.
   *
   * \param event The pending event.
   */
  virtual void process_event(const SDL_Event& event) = 0;

  /**
   * Returns the configured options for the renderer.
   *
   * \return
   * The renderer options.
   */
  [[nodiscard]]
  virtual auto get_options() -> const RendererOptions& = 0;
};

}  // namespace tactile
