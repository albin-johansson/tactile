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

  Runtime();

  ~Runtime() noexcept;

  void init_window(uint32 flags);

  void set_renderer(IRenderer* renderer);

  void set_compression_provider(CompressionType format,
                                ICompressionProvider* provider);

  auto start() -> int;

  [[nodiscard]]
  auto get_window() -> IWindow*;

  [[nodiscard]]
  auto get_renderer() -> IRenderer*;

  [[nodiscard]]
  auto get_imgui_context() -> ImGuiContext*;

 private:
  struct Data;
  Unique<Data> mData;
};

}  // namespace tactile
