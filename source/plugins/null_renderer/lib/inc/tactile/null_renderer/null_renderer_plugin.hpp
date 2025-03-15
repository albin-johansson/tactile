// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/basic/smart_ptr.hpp"
#include "tactile/core/plugin/plugin.hpp"
#include "tactile/core/plugin/renderer.hpp"
#include "tactile/null_renderer/api.hpp"

namespace tactile::null_renderer {

class TACTILE_NULL_RENDERER_API NullRendererPlugin final : public IPlugin
{
 public:
  auto install(IRuntime& runtime) -> Result<void> override;

  void uninstall(IRuntime& runtime) override;

 private:
  Unique<IRenderer> m_renderer {};
};

extern "C"
{
  TACTILE_NULL_RENDERER_API auto tactile_create_plugin() -> IPlugin*;
  TACTILE_NULL_RENDERER_API void tactile_destroy_plugin(IPlugin* plugin);
}

}  // namespace tactile::null_renderer
