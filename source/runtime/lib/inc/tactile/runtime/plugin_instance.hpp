// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/runtime/api.hpp"
#include "tactile/runtime/plugin.hpp"

namespace tactile {

class IRuntime;
class IDynamicLibrary;

/**
 * Represents a loaded plugin instance.
 */
class TACTILE_RUNTIME_API PluginInstance final
{
 public:
  TACTILE_DELETE_COPY(PluginInstance);

  /**
   * Unloads the associated plugin.
   */
  ~PluginInstance() noexcept;

  PluginInstance(PluginInstance&& other) noexcept;

  auto operator=(PluginInstance&&) -> PluginInstance& = delete;

  /**
   * Attempts to load a plugin.
   *
   * \details
   * This function will first try to load a shared library with the specified
   * name. Then, the "constructor" and "destructor" functions will be looked up
   * in the library (see \c PluginConstructor and \c PluginDestructor), which
   * must be called \c tactile_make_plugin and \c tactile_free_plugin,
   * respectively. The constructor function is then used to create the actual
   * plugin instance, which is then finally initialized via \c IPlugin::load.
   *
   * \details
   * The plugin is unregistered via \c IPlugin::unload and subsequently
   * destroyed when the \c PluginInstance goes out of scope.
   *
   * \param runtime     The associated runtime.
   * \param plugin_name The file name of the plugin module.
   *
   * \return
   * A plugin instance if successful; an empty optional otherwise.
   */
  [[nodiscard]]
  static auto load(IRuntime* runtime, StringView plugin_name) -> Optional<PluginInstance>;

 private:
  IRuntime* mRuntime;
  Unique<IDynamicLibrary> mDLL;
  PluginDestructor* mPluginDestructor;
  IPlugin* mPlugin;
  bool mPrimed;

  PluginInstance(IRuntime* runtime,
                 Unique<IDynamicLibrary> dll,
                 PluginDestructor* plugin_destructor,
                 IPlugin* plugin);
};

}  // namespace tactile
