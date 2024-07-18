// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"
#include "tactile/base/io/compress/compression_format.hpp"
#include "tactile/base/io/save/save_format_id.hpp"
#include "tactile/base/prelude.hpp"

struct ImGuiContext;

namespace tactile {

class IWindow;
class IRenderer;
class ICompressor;
class ISaveFormat;

/**
 * Interface that defines the primary API used by dynamic Tactile modules.
 */
class IRuntime
{
 public:
  TACTILE_INTERFACE_CLASS(IRuntime);

  /**
   * Initializes the application window.
   *
   * \param flags Additional flags forwarded to the \c SDL_CreateWindow
   *              function. Some flags are automatically added, the one thing
   *              renderer plugins must submit here is the flag that tells
   *              SDL which type of window to create. For example,
   *              \c SDL_WINDOW_OPENGL for an OpenGL window.
   */
  virtual void init_window(uint32 flags) = 0;

  /**
   * Sets the associated renderer.
   *
   * \param renderer The renderer to use.
   */
  virtual void set_renderer(IRenderer* renderer) = 0;

  /**
   * Sets the compression format implementation for a given format.
   *
   * \param id     The compression format identifier.
   * \param format The compression format implementation. The compression format is
   *               unregistered for the given format if the pointer is null.
   */
  virtual void set_compression_format(CompressionFormat id, ICompressor* format) = 0;

  /**
   * Sets the save format implementation for a given format.
   *
   * \param id     The save format identifier.
   * \param format The save format implementation. The save format is
   *               unregistered for the given format if the pointer is null.
   */
  virtual void set_save_format(SaveFormatId id, ISaveFormat* format) = 0;

  /**
   * Returns the application window.
   *
   * \return
   * A possibly null pointer to the application window.
   */
  [[nodiscard]]
  virtual auto get_window() -> IWindow* = 0;

  /**
   * Returns the associated renderer.
   *
   * \return
   * A possibly null pointer to the renderer.
   */
  [[nodiscard]]
  virtual auto get_renderer() -> IRenderer* = 0;

  /**
   * Returns the compression format registered with a given format identifier.
   *
   * \param id The compression format identifier.
   *
   * \return
   * A possibly null pointer to the compression format.
   */
  [[nodiscard]]
  virtual auto get_compression_format(CompressionFormat id) const -> const ICompressor* = 0;

  /**
   * Returns the save format registered with a given format identifier.
   *
   * \param id The save format identifier.
   *
   * \return
   * A possibly null pointer to the save format.
   */
  [[nodiscard]]
  virtual auto get_save_format(SaveFormatId id) const -> const ISaveFormat* = 0;

  /**
   * Returns the associated Dear ImGui context.
   *
   * \return
   * A Dear ImGui context.
   */
  [[nodiscard]]
  virtual auto get_imgui_context() -> ImGuiContext* = 0;
};

}  // namespace tactile
