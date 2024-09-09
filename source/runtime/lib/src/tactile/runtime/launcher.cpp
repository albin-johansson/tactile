// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/runtime/launcher.hpp"

#include <exception>  // exception
#include <utility>    // move
#include <vector>     // vector

#include <imgui.h>

#include "tactile/base/render/renderer.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/engine/engine.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/platform/dynamic_library.hpp"
#include "tactile/core/tactile_app.hpp"
#include "tactile/runtime/plugin_instance.hpp"
#include "tactile/runtime/runtime.hpp"

namespace tactile {
namespace launcher_impl {

[[nodiscard]]
auto get_plugin_names() -> std::vector<std::string_view>
{
  std::vector<std::string_view> plugin_names {};

#ifdef TACTILE_ENABLE_ZLIB_COMPRESSION
  plugin_names.emplace_back("tactile-zlib-compression" TACTILE_DLL_EXT);
#endif

#ifdef TACTILE_ENABLE_ZSTD_COMPRESSION
  plugin_names.emplace_back("tactile-zstd-compression" TACTILE_DLL_EXT);
#endif

#ifdef TACTILE_ENABLE_YAML_FORMAT
  plugin_names.emplace_back("tactile-yaml-format" TACTILE_DLL_EXT);
#endif

#ifdef TACTILE_ENABLE_TILED_TMJ_FORMAT
  plugin_names.emplace_back("tactile-tiled-tmj-format" TACTILE_DLL_EXT);
#endif

#ifdef TACTILE_ENABLE_TILED_TMX_FORMAT
  plugin_names.emplace_back("tactile-tiled-tmx-format" TACTILE_DLL_EXT);
#endif

#ifdef TACTILE_ENABLE_GODOT_TSCN_FORMAT
  plugin_names.emplace_back("tactile-godot-tscn-format" TACTILE_DLL_EXT);
#endif

#ifdef TACTILE_ENABLE_OPENGL_RENDERER
  plugin_names.emplace_back("tactile-opengl-renderer" TACTILE_DLL_EXT);
#elif defined(TACTILE_ENABLE_VULKAN_RENDERER)
  plugin_names.emplace_back("tactile-vulkan-renderer" TACTILE_DLL_EXT);
#else
  #error "No renderer has been configured"
#endif

  return plugin_names;
}

[[nodiscard]]
auto load_plugins(IRuntime& runtime) -> std::vector<PluginInstance>
{
  const auto plugin_names = get_plugin_names();

  std::vector<PluginInstance> plugins {};

  for (const auto plugin_name : plugin_names) {
    auto plugin = PluginInstance::load(&runtime, plugin_name);

    if (!plugin) {
      TACTILE_LOG_ERROR("Could not load plugin '{}'", plugin_name);
      continue;
    }

    plugins.push_back(std::move(*plugin));
  }

  return plugins;
}

}  // namespace launcher_impl

auto launch(const int, char*[]) -> int
{
  try {
    Runtime runtime {};

    TACTILE_LOG_INFO("Tactile " TACTILE_VERSION_STRING);

    const auto plugins [[maybe_unused]] = launcher_impl::load_plugins(runtime);

    auto* window = runtime.get_window();
    auto* renderer = runtime.get_renderer();

    if (window == nullptr) {
      TACTILE_LOG_ERROR("Window has not been initialized");
      return EXIT_FAILURE;
    }

    if (renderer == nullptr) {
      TACTILE_LOG_ERROR("A renderer has not been installed");
      return EXIT_FAILURE;
    }

    ImGuiMemAllocFunc imgui_alloc_fn {};
    ImGuiMemFreeFunc imgui_free_fn {};
    void* imgui_user_data {};
    runtime.get_imgui_allocator_functions(&imgui_alloc_fn, &imgui_free_fn, &imgui_user_data);

    ImGui::SetAllocatorFunctions(imgui_alloc_fn, imgui_free_fn, imgui_user_data);
    ImGui::SetCurrentContext(renderer->get_imgui_context());

    TactileApp app {&runtime};

    Engine engine {&app, renderer};
    engine.run();

    return EXIT_SUCCESS;
  }
  catch (const Exception& exception) {
    TACTILE_LOG_FATAL("Unhandled exception: {}\n{}", exception.what(), exception.trace());
  }
  catch (const std::exception& exception) {
    TACTILE_LOG_FATAL("Unhandled exception: {}", exception.what());
  }
  catch (...) {
    TACTILE_LOG_FATAL("Unhandled exception");
  }

  return EXIT_FAILURE;
}

}  // namespace tactile
