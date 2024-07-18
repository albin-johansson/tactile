// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/null_renderer/api.hpp"
#include "tactile/null_renderer/null_renderer.hpp"
#include "tactile/runtime/plugin.hpp"

namespace tactile {

/**
 * Manages the null renderer plugin.
 *
 * \note
 * This plugin is only intended for testing purposes.
 */
class TACTILE_NULL_RENDERER_API NullRendererPlugin final : public IPlugin
{
 public:
  void load(IRuntime& runtime) override;

  void unload(IRuntime& runtime) override;

 private:
  Unique<NullRenderer> mRenderer {};
};

extern "C"
{
  TACTILE_NULL_RENDERER_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_NULL_RENDERER_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
