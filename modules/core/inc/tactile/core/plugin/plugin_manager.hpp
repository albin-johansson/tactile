// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/platform/dynamic_library.hpp"
#include "tactile/foundation/plugin/plugin.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

using PluginCreateFn = IPlugin*();       ///< Signature of a plugin constructor.
using PluginDestroyFn = void(IPlugin*);  ///< Signature of a plugin destructor.
using PluginIdFn = const char*();        ///< Signature of a plugin ID getter.

/**
 * \brief Represents a loaded plugin instance.
 */
struct PluginInstance final {
  Unique<IDynamicLibrary> dll;  ///< The associated dynamic library.
  Managed<IPlugin> plugin;      ///< The loaded plugin.
  String id;                    ///< The plugin identifier.
};

/**
 * \brief Loads and manages a collection of runtime plugins.
 */
class TACTILE_CORE_API PluginManager final {
 public:
  TACTILE_DELETE_COPY(PluginManager);
  TACTILE_DEFAULT_MOVE(PluginManager);

  /**
   * \brief Locates and loads plugins, and puts them in a plugin manager.
   *
   * \param plugin_dir         the root plugin directory.
   * \param renderer_plugin_id the ID of the requested renderer plugin.
   *
   * \return a plugin manager.
   */
  [[nodiscard]]
  static auto load(const FilePath& plugin_dir, StringView renderer_plugin_id)
      -> Maybe<PluginManager>;

  /**
   * \brief Unloads all plugins.
   */
  ~PluginManager() noexcept;

 private:
  Vector<PluginInstance> mPlugins;

  explicit PluginManager(Vector<PluginInstance> plugins);

  [[nodiscard]]
  static auto _collect_plugins(const FilePath& dir) -> Vector<PluginInstance>;
};

}  // namespace tactile
