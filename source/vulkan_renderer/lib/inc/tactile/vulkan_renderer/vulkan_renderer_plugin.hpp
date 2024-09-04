// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <memory>  // unique_ptr

#include "tactile/base/prelude.hpp"
#include "tactile/base/util/scope_exit.hpp"
#include "tactile/runtime/plugin.hpp"
#include "tactile/vulkan_renderer/api.hpp"
#include "tactile/vulkan_renderer/vulkan_renderer.hpp"

namespace tactile {

class TACTILE_VULKAN_API VulkanRendererPlugin final : public IPlugin
{
 public:
  void load(IRuntime* runtime) override;

  void unload() override;

 private:
  IRuntime* m_runtime {};
  std::unique_ptr<VulkanRenderer> m_renderer {};
  ScopeExit m_sdl_vulkan_library_deleter {};
};

extern "C"
{
  TACTILE_VULKAN_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_VULKAN_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
