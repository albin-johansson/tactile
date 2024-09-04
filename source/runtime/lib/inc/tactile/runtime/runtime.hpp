// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <memory>  // unique_ptr

#include "tactile/base/int.hpp"
#include "tactile/base/io/compress/compression_format.hpp"
#include "tactile/base/io/save/save_format_id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/runtime.hpp"
#include "tactile/runtime/api.hpp"

struct ImGuiContext;

namespace tactile {

/**
 * Implements the runtime interface.
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

 private:
  struct Data;
  std::unique_ptr<Data> mData;
};

TACTILE_RUNTIME_API auto runtime_malloc(std::size_t bytes) noexcept -> void*;
TACTILE_RUNTIME_API void runtime_free(void* memory) noexcept;

}  // namespace tactile
