// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/runtime/plugin.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

class TACTILE_VULKAN_API VulkanRendererPlugin final : public IPlugin
{
 public:
  void load(IRuntime& runtime) override;

  void unload(IRuntime& runtime) override;
};

extern "C"
{
  TACTILE_VULKAN_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_VULKAN_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
