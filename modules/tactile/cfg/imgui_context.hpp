#pragma once

#include <centurion.hpp>

namespace Tactile {

/// Handles the configuration of the Dear ImGui context.
class ImGuiContext final {
 public:
  ImGuiContext(cen::Window& window, cen::GLContext& context);

  ~ImGuiContext();

 private:
  /* This is used as a workaround to annoying unit test issues for Windows CI builds */
  bool mInitializedBackend{};
};

}  // namespace Tactile
