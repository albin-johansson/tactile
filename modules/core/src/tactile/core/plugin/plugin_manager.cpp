// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/plugin/plugin_manager.hpp"

#include <filesystem>  // is_directory
#include <utility>     // move

#include "tactile/core/debug/log/logger.hpp"

namespace tactile {

auto PluginManager::get() -> PluginManager&
{
  static PluginManager plugin_manager;
  return plugin_manager;
}

void PluginManager::scan(const FilePath& dir)
{
  if (!std::filesystem::is_directory(dir)) {
    TACTILE_LOG_ERROR("Plugin path was not a directory");
    return;
  }

  for (const auto& dir_entry : DirectoryIterator {dir}) {
    const auto& dir_entry_path = dir_entry.path();

    PluginData plugin_data {};
    plugin_data.dll = load_library(dir_entry_path);

    if (plugin_data.dll != nullptr) {
      plugin_data.plugin = load_plugin(*plugin_data.dll);

      if (plugin_data.plugin != nullptr) {
        TACTILE_LOG_DEBUG("Found plugin {}",
                          plugin_data.dll->get_path().filename().string());
        mPlugins.push_back(std::move(plugin_data));
      }
    }
  }
}

auto PluginManager::get_plugins() -> Vector<PluginData>&
{
  return mPlugins;
}

auto PluginManager::get_plugins() const -> const Vector<PluginData>&
{
  return mPlugins;
}

auto PluginManager::load_plugin(const IDynamicLibrary& lib) -> Managed<IPlugin>
{
  auto* create_plugin = get_symbol<PluginCreateFn>(lib, "tactile_create_plugin");
  auto* destroy_plugin = get_symbol<PluginDestroyFn>(lib, "tactile_destroy_plugin");

  if (create_plugin && destroy_plugin) {
    if (auto* plugin = create_plugin()) {
      return Managed<IPlugin> {plugin, destroy_plugin};
    }
  }

  return Managed<IPlugin> {nullptr, nullptr};
}

}  // namespace tactile
