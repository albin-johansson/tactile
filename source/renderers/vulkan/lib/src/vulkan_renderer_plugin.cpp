// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/vulkan/vulkan_renderer_plugin.hpp"

#include <new>  // nothrow

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <imgui.h>

#include "tactile/base/runtime/runtime.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile {

void VulkanRendererPlugin::load(IRuntime* runtime)
{
  runtime::log(LogLevel::kTrace, "Loading Vulkan renderer plugin");
  m_runtime = runtime;

  try {
    if (SDL_Vulkan_LoadLibrary(nullptr) == -1) {
      runtime::log(LogLevel::kError, "Could not load Vulkan library: {}", SDL_GetError());
      return;
    }

    m_sdl_vulkan_library_deleter = ScopeExit {[] { SDL_Vulkan_UnloadLibrary(); }};

    m_runtime->init_window(SDL_WINDOW_VULKAN);
    auto* window = m_runtime->get_window();
    if (!window) {
      runtime::log(LogLevel::kError, "Could not initialize Vulkan window");
      return;
    }

    ImGuiMemAllocFunc imgui_alloc_fn {};
    ImGuiMemFreeFunc imgui_free_fn {};
    void* imgui_user_data {};
    m_runtime->get_imgui_allocator_functions(&imgui_alloc_fn,
                                             &imgui_free_fn,
                                             &imgui_user_data);
    ImGui::SetAllocatorFunctions(imgui_alloc_fn, imgui_free_fn, imgui_user_data);

    m_renderer = std::make_unique<VulkanRenderer>(m_runtime->get_renderer_options(), window);
    m_runtime->set_renderer(m_renderer.get());
  }
  catch (...) {
    runtime::log(LogLevel::kError, "Could not load Vulkan renderer");
  }
}

void VulkanRendererPlugin::unload()
{
  runtime::log(LogLevel::kTrace, "Unloading Vulkan renderer plugin");

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
