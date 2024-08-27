// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_renderer_plugin.hpp"

#include <new>  // nothrow

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <volk.h>

#include "tactile/base/runtime.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

void VulkanRendererPlugin::load(IRuntime* runtime)
{
  log(LogLevel::kTrace, "Loading Vulkan renderer plugin");
  mRuntime = runtime;

  try {
    if (SDL_Vulkan_LoadLibrary(nullptr) == -1) {
      log(LogLevel::kError, "Could not load Vulkan library: {}", SDL_GetError());
      return;
    }

    volkInitializeCustom(
        reinterpret_cast<PFN_vkGetInstanceProcAddr>(SDL_Vulkan_GetVkGetInstanceProcAddr()));

  }
  catch (...) {
    log(LogLevel::kError, "Could not load Vulkan renderer");
  }
}

void VulkanRendererPlugin::unload()
{
  log(LogLevel::kTrace, "Unloading Vulkan renderer plugin");
  mRuntime = nullptr;
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
