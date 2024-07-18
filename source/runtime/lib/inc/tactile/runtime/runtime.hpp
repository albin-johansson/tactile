// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <format>  // format_args, make_format_args

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/io/compress/compression_format.hpp"
#include "tactile/base/io/save/save_format_id.hpp"
#include "tactile/base/log/log_level.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/runtime.hpp"
#include "tactile/runtime/api.hpp"

struct ImGuiContext;

namespace tactile {

/**
 * Provides the primary API used by dynamic Tactile modules.
 */
class TACTILE_RUNTIME_API Runtime final : public IRuntime
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

  ~Runtime() noexcept override;

  /**
   * Logs a message using the internal logger.
   *
   * \tparam Args The format argument types.
   *
   * \param level The severity of the message.
   * \param fmt   The format string.
   * \param args  The format arguments.
   */
  template <typename... Args>
  static void log(const LogLevel level, const StringView fmt, const Args&... args)
  {
    _log(level, fmt, std::make_format_args(args...));
  }

  void init_window(uint32 flags) override;

  void set_renderer(IRenderer* renderer) override;

  void set_compression_format(CompressionFormat id, ICompressor* format) override;

  void set_save_format(SaveFormatId id, ISaveFormat* format) override;

  [[nodiscard]]
  auto get_window() -> IWindow* override;

  [[nodiscard]]
  auto get_renderer() -> IRenderer* override;

  [[nodiscard]]
  auto get_compression_format(CompressionFormat id) const -> const ICompressor* override;

  [[nodiscard]]
  auto get_save_format(SaveFormatId id) const -> const ISaveFormat* override;

  [[nodiscard]]
  auto get_imgui_context() -> ImGuiContext* override;

 private:
  struct Data;
  Unique<Data> mData;

  static void _log(LogLevel level, StringView fmt, std::format_args args);
};

}  // namespace tactile
