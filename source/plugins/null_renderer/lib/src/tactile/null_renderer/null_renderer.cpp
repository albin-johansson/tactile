// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/null_renderer/null_renderer.hpp"

namespace tactile::null_renderer {

auto NullRenderer::begin_frame() -> Result<void>
{
  return ok();
}

void NullRenderer::end_frame()
{}

void NullRenderer::reload_fonts()
{}

void NullRenderer::process_event(const SDL_Event&)
{}

}  // namespace tactile::null_renderer
