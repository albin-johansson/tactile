#pragma once

#include <centurion.hpp>
#include <tactile_def.hpp>

namespace Tactile {

/// Handles the configuration of the Dear ImGui context.
class ImGuiContext final {
 public:
  ImGuiContext(cen::Window& window, cen::GLContext& context);

  ~ImGuiContext();

  TACTILE_DEFAULT_COPY(ImGuiContext)
  TACTILE_DEFAULT_MOVE(ImGuiContext)

 private:
  /* This is used as a workaround to annoying unit test issues for Windows CI builds */
  bool mInitializedBackend{};
};

}  // namespace Tactile
