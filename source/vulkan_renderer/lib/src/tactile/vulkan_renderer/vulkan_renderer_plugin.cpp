// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_renderer_plugin.hpp"

#include <new>  // nothrow

#include "tactile/runtime/runtime.hpp"

namespace tactile {

void VulkanRendererPlugin::load(Runtime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Loading Vulkan renderer plugin");
}

void VulkanRendererPlugin::unload(Runtime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Unloading Vulkan renderer plugin");
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) VulkanRendererPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
