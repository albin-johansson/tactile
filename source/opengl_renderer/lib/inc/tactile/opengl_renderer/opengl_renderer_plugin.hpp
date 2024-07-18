// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/opengl_renderer/api.hpp"
#include "tactile/opengl_renderer/opengl_renderer.hpp"
#include "tactile/runtime/plugin.hpp"

namespace tactile {

/**
 * Manages the OpenGL renderer plugin.
 */
class TACTILE_OPENGL_API OpenGLRendererPlugin final : public IPlugin
{
 public:
  void load(IRuntime& runtime) override;

  void unload(IRuntime& runtime) override;

 private:
  Optional<OpenGLRenderer> mRenderer {};
};

extern "C"
{
  TACTILE_OPENGL_API auto tactile_make_plugin() -> IPlugin*;

  TACTILE_OPENGL_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
