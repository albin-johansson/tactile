// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/plugin/renderer.hpp"
#include "tactile/null_renderer/api.hpp"

namespace tactile::null_renderer {

class TACTILE_NULL_RENDERER_API NullRenderer final : public IRenderer
{
 public:
  [[nodiscard]]
  auto begin_frame() -> Result<void> override;

  void end_frame() override;

  void reload_fonts() override;

  void process_event(const SDL_Event& event) override;
};

}  // namespace tactile::null_renderer
