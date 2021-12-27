#pragma once

#include <centurion.hpp>

namespace Tactile {

/// Handles the configuration of the Dear ImGui context.
class ImGuiContext final {
 public:
  ImGuiContext(cen::Window& window, cen::GLContext& context);

  ~ImGuiContext();
};

}  // namespace Tactile
