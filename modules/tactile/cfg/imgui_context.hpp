#pragma once

#include <centurion.hpp>
#include <tactile_def.hpp>

namespace tactile {

/**
 * \brief Handles the configuration of the Dear ImGui context.
 *
 * \ingroup cfg
 */
class im_gui_context final {
 public:
  im_gui_context(cen::window& window, cen::gl_context& context);

  ~im_gui_context();

  TACTILE_DEFAULT_COPY(im_gui_context)
  TACTILE_DEFAULT_MOVE(im_gui_context)

 private:
  /* This is used as a workaround to annoying unit test issues for Windows CI builds */
  bool mInitializedBackend{};
};

}  // namespace tactile
