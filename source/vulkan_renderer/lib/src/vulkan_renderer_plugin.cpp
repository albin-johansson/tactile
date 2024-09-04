// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan_renderer/vulkan_renderer_plugin.hpp"

#include <new>  // nothrow

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <imgui.h>
#include <vulkan/vulkan.h>

#include "tactile/base/runtime.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/runtime/runtime.hpp"
#include "tactile/vulkan_renderer/vulkan_util.hpp"

namespace tactile {

void VulkanRendererPlugin::load(IRuntime* runtime)
{
  log(LogLevel::kTrace, "Loading Vulkan renderer plugin");
  m_runtime = runtime;

  try {
    if (SDL_Vulkan_LoadLibrary(nullptr) == -1) {
      log(LogLevel::kError, "Could not load Vulkan library: {}", SDL_GetError());
      return;
    }

    m_sdl_vulkan_library_deleter = ScopeExit {[] { SDL_Vulkan_UnloadLibrary(); }};

    m_runtime->init_window(SDL_WINDOW_VULKAN);
    auto* window = m_runtime->get_window();
    if (!window) {
      log(LogLevel::kError, "Could not initialize Vulkan window");
      return;
    }

    ImGui::SetAllocatorFunctions([](const usize size, void*) { return runtime_malloc(size); },
                                 [](void* ptr, void*) { runtime_free(ptr); });

    m_renderer = std::make_unique<VulkanRenderer>(window);
    m_runtime->set_renderer(m_renderer.get());
  }
  catch (...) {
    log(LogLevel::kError, "Could not load Vulkan renderer");
  }
}

void VulkanRendererPlugin::unload()
{
  log(LogLevel::kTrace, "Unloading Vulkan renderer plugin");

  m_runtime->set_renderer(nullptr);
  m_runtime = nullptr;

  m_renderer.reset();
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
