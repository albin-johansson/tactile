#pragma once

#include <centurion.hpp>
#include <tactile_def.hpp>

namespace tactile {

/// Handles the configuration of the Dear ImGui context.
class ImGuiContext final {
 public:
  ImGuiContext(cen::window& window, cen::gl_context& context);

  ~ImGuiContext();

  TACTILE_DEFAULT_COPY(ImGuiContext)
  TACTILE_DEFAULT_MOVE(ImGuiContext)

 private:
  /* This is used as a workaround to annoying unit test issues for Windows CI builds */
  bool mInitializedBackend{};
};

}  // namespace tactile
