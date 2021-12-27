#pragma once

#include <centurion.hpp>

namespace Tactile {

/// Handles the configuration of the Dear ImGui context.
class ImGuiContext final {
 public:
  ImGuiContext(cen::window& window, cen::gl_context& context);

  ~ImGuiContext();
};

}  // namespace Tactile
