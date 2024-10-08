// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot_tscn/godot_scene_format_plugin.hpp"

#include <new>  // nothrow

#include "tactile/base/runtime.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile::godot {

void GodotSceneFormatPlugin::load(IRuntime* runtime)
{
  log(LogLevel::kTrace, "Loading Godot TSCN format plugin");
  m_runtime = runtime;

  m_format = std::make_unique<GodotSceneFormat>(m_runtime);
  m_runtime->set_save_format(SaveFormatId::kGodotTscn, m_format.get());
}

void GodotSceneFormatPlugin::unload()
{
  log(LogLevel::kTrace, "Unloading Godot TSCN format plugin");

  m_runtime->set_save_format(SaveFormatId::kGodotTscn, nullptr);
  m_format.reset();

  m_runtime = nullptr;
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) GodotSceneFormatPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile::godot
