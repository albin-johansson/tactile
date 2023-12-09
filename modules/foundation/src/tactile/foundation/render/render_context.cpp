// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/render/render_context.hpp"

#include "tactile/foundation/log/logger.hpp"

namespace tactile {

auto RenderContext::get() noexcept -> RenderContext&
{
  static RenderContext render_context {};
  return render_context;
}

void RenderContext::set_window(IWindow* window) noexcept
{
  TACTILE_LOG_DEBUG("Setting render context window to {}",
                    static_cast<const void*>(window));
  mWindow = window;
}

void RenderContext::set_renderer(IRenderer* renderer) noexcept
{
  TACTILE_LOG_DEBUG("Setting render context renderer to {}",
                    static_cast<const void*>(renderer));
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
