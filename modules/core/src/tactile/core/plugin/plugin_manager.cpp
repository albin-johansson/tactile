// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/plugin/plugin_manager.hpp"

#include <filesystem>  // is_directory
#include <utility>     // move

#include "tactile/core/debug/error.hpp"
#include "tactile/core/debug/log/logger.hpp"

namespace tactile {

auto PluginManager::get() -> PluginManager&
{
  static PluginManager plugin_manager;
  return plugin_manager;
}

void PluginManager::scan(const fs::Path& dir)
{
  if (!std::filesystem::is_directory(dir)) {
    TACTILE_LOG_ERROR("Plugin path was not a directory");
    return;
  }

  for (const auto& dir_entry : fs::DirectoryIterator {dir}) {
    const auto& dir_entry_path = dir_entry.path();
    if (auto plugin_info = load_library_info(dir_entry_path)) {
      TACTILE_LOG_DEBUG("Found plugin {}", plugin_info->name);
      mPlugins.push_back(std::move(*plugin_info));
    }
  }
}

auto PluginManager::get_plugins() -> Vector<PluginInfo>&
{
  return mPlugins;
}

auto PluginManager::get_plugins() const -> const Vector<PluginInfo>&
{
  return mPlugins;
}

auto PluginManager::is_dll(const fs::Path& file) -> bool
{
  const auto extension = file.extension();

  if constexpr (kIsLinux) {
    return extension == ".so";
  }
  else if constexpr (kIsApple) {
    return extension == ".so" || extension == ".dylib";
  }
  else if constexpr (kIsWindows) {
    return extension == ".dll";
  }

  TACTILE_LOG_WARN("Unknown platform: cannot determine valid DLL extensions");
  return false;
}

auto PluginManager::load_library_info(const fs::Path& path) -> Maybe<PluginInfo>
{
  if (!is_dll(path)) {
    return kNone;
  }

  PluginInfo plugin_info;
  plugin_info.dll = load_library(path);

  if (plugin_info.dll) {
    plugin_info.name = path.filename().string();
    plugin_info.library_path = path;

    const auto& dll = *plugin_info.dll;
    auto* plugin_factory = get_symbol<PluginCreateFn>(dll, "tactile_create_plugin");
    auto* plugin_deleter = get_symbol<PluginDestroyFn>(dll, "tactile_destroy_plugin");

    if (plugin_factory && plugin_deleter) {
      if (auto* plugin = plugin_factory()) {
        plugin_info.plugin = make_unique_foreign(plugin, plugin_deleter);
        return plugin_info;
      }
    }
  }

  return kNone;
}

}  // namespace tactile
