#pragma once

#include <centurion.hpp>  // window, gl::context

namespace tactile {

class ImGuiWrapper
{
 public:
  ImGuiWrapper(cen::window& window, cen::gl_context& context);

  ~ImGuiWrapper();
};

}  // namespace tactile
