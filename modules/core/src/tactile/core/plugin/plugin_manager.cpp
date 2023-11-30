// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/plugin/plugin_manager.hpp"

#include <filesystem>  // is_directory
#include <iterator>    // make_move_iterator
#include <utility>     // move

#include "tactile/core/debug/log/logger.hpp"

namespace tactile {

auto PluginManager::load(const FilePath& plugin_dir, const StringView renderer_plugin_id)
    -> Maybe<PluginManager>
{
  if (!std::filesystem::is_directory(plugin_dir)) {
    TACTILE_LOG_ERROR("Plugin path was not a directory");
    return kNothing;
  }

  Vector<PluginInstance> plugin_instances;

  for (auto& renderer_plugin : _collect_plugins(plugin_dir / "renderers")) {
    if (renderer_plugin.id == renderer_plugin_id) {
      plugin_instances.push_back(std::move(renderer_plugin));
      break;
    }
  }

  if (plugin_instances.empty()) {
    TACTILE_LOG_ERROR("Could not find renderer plugin '{}'", renderer_plugin_id);
    return kNothing;
  }

  auto save_format_plugins = _collect_plugins(plugin_dir / "formats");
  plugin_instances.insert(plugin_instances.end(),
                          std::make_move_iterator(save_format_plugins.begin()),
                          std::make_move_iterator(save_format_plugins.end()));

  return PluginManager {std::move(plugin_instances)};
}

PluginManager::PluginManager(Vector<PluginInstance> plugins)
  : mPlugins {std::move(plugins)}
{
  TACTILE_LOG_DEBUG("Loading plugins...");

  for (const auto& plugin_instance : mPlugins) {
    plugin_instance.plugin->on_load();
  }
}

PluginManager::~PluginManager() noexcept
{
  TACTILE_LOG_DEBUG("Unloading plugins...");

  for (const auto& plugin_instance : mPlugins) {
    plugin_instance.plugin->on_unload();
  }
}

auto PluginManager::_collect_plugins(const FilePath& dir) -> Vector<PluginInstance>
{
  Vector<PluginInstance> plugin_instances;

  for (const auto& dir_entry : DirectoryIterator {dir}) {
    if (auto dll = load_library(dir_entry.path())) {
      auto* create_plugin = get_symbol<PluginCreateFn>(*dll, "tactile_create_plugin");
      auto* destroy_plugin = get_symbol<PluginDestroyFn>(*dll, "tactile_destroy_plugin");
      auto* get_id = get_symbol<PluginIdFn>(*dll, "tactile_plugin_id");

      if (create_plugin && destroy_plugin && get_id) {
        if (const char* plugin_id = get_id()) {
          TACTILE_LOG_DEBUG("Found plugin '{}'", plugin_id);

          Managed<IPlugin> plugin {create_plugin(), destroy_plugin};
          if (plugin != nullptr) {
            plugin_instances.push_back(PluginInstance {
              .dll = std::move(dll),
              .plugin = std::move(plugin),
              .id = String {plugin_id},
            });
          }
          else {
            TACTILE_LOG_ERROR("Could not initialize plugin '{}'", plugin_id);
          }
        }
        else {
          TACTILE_LOG_WARN("Detected plugin with invalid identifier");
        }
      }
    }
  }

  return plugin_instances;
}

}  // namespace tactile
