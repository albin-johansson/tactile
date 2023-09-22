// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/platform/dynamic_library.hpp"
#include "tactile/core/plugin/plugin.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

using PluginCreateFn = IPlugin* (*) ();      ///< Signature of a plugin "constructor".
using PluginDestroyFn = void (*)(IPlugin*);  ///< Signature of a plugin "destructor".

/**
 * \brief Provides information about a dynamic library plugin.
 */
struct PluginInfo final {
  String name;                                       ///< The plugin name.
  fs::Path library_path;                             ///< The file path of the binary.
  Unique<IDynamicLibrary> dll;                       ///< The dynamic library handle.
  UniqueForeign<IPlugin> plugin {nullptr, nullptr};  ///< The plugin instance.
};

/**
 * \brief Manages a collection of runtime plugins.
 */
class PluginManager final {
 public:
  /**
   * \brief Returns the global plugin manager.
   *
   * \return the plugin manager.
   */
  [[nodiscard]]
  TACTILE_CORE_API static auto get() -> PluginManager&;

  /**
   * \brief Iterates the specified directory for potential plugins and loads their info.
   *
   * \details This function only iterates the given directory, and loads information about
   *          found plugins using `load_library_info`. The information is subsequently
   *          stored in the plugin manager itself.
   *
   * \param dir the plugin directory path.
   */
  TACTILE_CORE_API void scan(const fs::Path& dir);

  /**
   * \brief Returns the loaded plugin information.
   *
   * \return the plugin information data.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto get_plugins() -> Vector<PluginInfo>&;

  /** \copydoc PluginManager::get_plugins */
  [[nodiscard]]
  TACTILE_CORE_API auto get_plugins() const -> const Vector<PluginInfo>&;

  /**
   * \brief Indicates whether a file is likely to be a dynamic library.
   *
   * \note This function merely provides a course heuristic useful for excluding files
   *       that are unlikely to be dynamic libraries. It works by simply checking the file
   *       extension of the specified file and compares the extension to established
   *       dynamic library file extensions for the current platform.
   *
   * \param file the file to check.
   *
   * \return true if the file could be a dynamic library; false otherwise.
   */
  [[nodiscard]]
  TACTILE_CORE_API static auto is_dll(const fs::Path& file) -> bool;

  /**
   * \brief Obtains information about a plugin.
   *
   * \details This function will, on success, initialize all members of the `PluginInfo`
   *          struct. However, the plugin will not be explicitly initialized, i.e., you'll
   *          still need to call `IPlugin::on_load` (and `IPlugin::on_unload`) yourself.
   *          This makes it possible to selectively load plugins based on user input.
   *
   * \param path the file path to the dynamic library.
   *
   * \return the plugin information, or nothing on failure.
   */
  [[nodiscard]]
  TACTILE_CORE_API static auto load_library_info(const fs::Path& path)
      -> Maybe<PluginInfo>;

 private:
  Vector<PluginInfo> mPlugins;
};

}  // namespace tactile
