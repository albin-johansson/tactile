#pragma once

#include <centurion.hpp>  // window, gl::context

namespace Tactile {

class ImGuiContext final
{
 public:
  ImGuiContext(cen::window& window, cen::gl_context& context);

  ~ImGuiContext();
};

}  // namespace Tactile
