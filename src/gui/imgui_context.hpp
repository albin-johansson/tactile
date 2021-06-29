#pragma once

#include <centurion.hpp>  // window, gl::context

namespace tactile {

class ImGuiContext final
{
 public:
  ImGuiContext(cen::window& window, cen::gl_context& context);

  ~ImGuiContext();
};

}  // namespace tactile
