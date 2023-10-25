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
struct PluginData final {
  Unique<IDynamicLibrary> dll;                 ///< The dynamic library handle.
  Managed<IPlugin> plugin {nullptr, nullptr};  ///< The plugin instance.
};

/**
 * \brief Manages a collection of runtime plugins.
 */
class TACTILE_CORE_API PluginManager final {
 public:
  PluginManager() = default;
  ~PluginManager() noexcept = default;

  TACTILE_DELETE_COPY(PluginManager);
  TACTILE_DEFAULT_MOVE(PluginManager);

  /**
   * \brief Returns the global plugin manager.
   *
   * \return the plugin manager.
   */
  [[nodiscard]]
  static auto get() -> PluginManager&;

  /**
   * \brief Iterates the specified directory for potential plugins and loads their info.
   *
   * \details This function only iterates the given directory, and loads information about
   *          found plugins using `load_library_info`. The information is subsequently
   *          stored in the plugin manager itself.
   *
   * \param dir the plugin directory path.
   */
  void scan(const FilePath& dir);

  [[nodiscard]]
  auto get_plugins() -> Vector<PluginData>&;

  [[nodiscard]]
  auto get_plugins() const -> const Vector<PluginData>&;

  /**
   * \brief Tries to load a plugin from a dynamic library.
   *
   * \details Plugin libraries must provide two C-linkage functions:
   *          `tactile_create_plugin` and `tactile_destroy_plugin`. The signatures of
   *          these functions must be equivalent to `IPlugin*()` and `void(IPlugin*)`,
   *          respectively.
   *
   * \param lib the source dynamic library handle.
   *
   * \return a potentially null managed plugin pointer.
   */
  [[nodiscard]]
  static auto load_plugin(const IDynamicLibrary& lib) -> Managed<IPlugin>;

 private:
  Vector<PluginData> mPlugins;
};

}  // namespace tactile
