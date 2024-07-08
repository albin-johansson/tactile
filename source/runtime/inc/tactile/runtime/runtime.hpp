// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/io/compress/compression_type.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/runtime/api.hpp"

struct ImGuiContext;

namespace tactile {

class IWindow;
class IRenderer;
class ICompressionProvider;

/**
 * Provides the primary API used by dynamic Tactile modules.
 */
class TACTILE_RUNTIME_API Runtime final
{
 public:
  TACTILE_DELETE_COPY(Runtime);
  TACTILE_DELETE_MOVE(Runtime);

  /**
   * Performs basic initialization of the runtime.
   *
   * \details
   * This constructor performs the below steps. The only things that must be
   * manually configured is the application window (via \c init_window) and the
   * renderer (via \c set_renderer). These are expected to be configured by
   * renderer plugins, in their \c IPlugin::load implementations. \n
   *
   * - Installs a custom terminate handler. \n
   * - Initializes the Protobuf library. \n
   * - Initializes The SDL library. \n
   * - Creates and installs a core logger. \n
   * - Creates a Dear ImGui context.
   */
  Runtime();

  ~Runtime() noexcept;

  /**
   * Initializes the application window.
   *
   * \param flags Additional flags forwarded to the \c SDL_CreateWindow
   *              function. Some flags are automatically added, the one thing
   *              renderer plugins must submit here is the flag that tells
   *              SDL which type of window to create. For example,
   *              \c SDL_WINDOW_OPENGL for an OpenGL window.
   */
  void init_window(uint32 flags);

  /**
   * Sets the associated renderer.
   *
   * \param renderer The renderer to use.
   */
  void set_renderer(IRenderer* renderer);

  /**
   * Sets a compressor for a given compression format.
   *
   * \param format     The associated compression format.
   * \param compressor The compressor to associate with the format. The
   *                   compressor is unregistered for the given format if this
   *                   is null.
   */
  void set_compression_provider(CompressionType format,
                                ICompressionProvider* compressor);

  /**
   * Attempts to start the application loop.
   *
   * \note
   * A window and renderer must have been configured before this function is
   * called.
   *
   * \return
   * \c EXIT_SUCCESS if successful; \c EXIT_FAILURE otherwise.
   */
  auto start() -> int;

  /**
   * Returns the application window.
   *
   * \return
   * A possibly null pointer to the application window.
   */
  [[nodiscard]]
  auto get_window() -> IWindow*;

  /**
   * Returns the associated renderer.
   *
   * \return
   * A possibly null pointer to the renderer.
   */
  [[nodiscard]]
  auto get_renderer() -> IRenderer*;

  /**
   * Returns the associated Dear ImGui context.
   *
   * \return
   * A Dear ImGui context.
   */
  [[nodiscard]]
  auto get_imgui_context() -> ImGuiContext*;

 private:
  struct Data;
  Unique<Data> mData;
};

}  // namespace tactile
