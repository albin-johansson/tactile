#pragma once

#include <centurion.hpp>  // window, gl::context

namespace Tactile {

/**
 * \brief A simple wrapper that manages a Dear ImGui context.
 *
 * \ingroup gui
 */
class ImGuiContext final
{
 public:
  ImGuiContext(cen::window& window, cen::gl_context& context);

  ~ImGuiContext();
};

}  // namespace Tactile
