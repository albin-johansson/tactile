// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/render/render_context.hpp"

namespace tactile {

auto RenderContext::get() noexcept -> RenderContext&
{
  static RenderContext render_context {};
  return render_context;
}

void RenderContext::set_window(IWindow* window) noexcept
{
  mWindow = window;
}

void RenderContext::set_renderer(IRenderer* renderer) noexcept
{
  mRenderer = renderer;
}

auto RenderContext::get_window() noexcept -> IWindow*
{
  return mWindow;
}

auto RenderContext::get_renderer() noexcept -> IRenderer*
{
  return mRenderer;
}

}  // namespace tactile
