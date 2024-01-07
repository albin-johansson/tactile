// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/plugin/plugin.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/opengl-renderer/api.hpp"
#include "tactile/opengl-renderer/opengl_renderer.hpp"
#include "tactile/opengl-renderer/opengl_window.hpp"

namespace tactile::opengl {

/**
 * A renderer plugin based on OpenGL 4.1.0.
 */
class TACTILE_OPENGL_API OpenGLPlugin final : public IPlugin {
 public:
  void on_load() override;

  void on_unload() noexcept override;

 private:
  Unique<OpenGLWindow> mWindow;
  Unique<OpenGLRenderer> mRenderer;
};

extern "C"
{
  [[nodiscard]]
  TACTILE_OPENGL_API auto tactile_create_plugin() -> IPlugin*;

  TACTILE_OPENGL_API void tactile_destroy_plugin(IPlugin* plugin);

  [[nodiscard]]
  TACTILE_OPENGL_API auto tactile_plugin_id() -> const char*;
}

}  // namespace tactile::opengl
