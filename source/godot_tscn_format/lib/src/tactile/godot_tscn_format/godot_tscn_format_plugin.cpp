// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot_tscn_format/godot_tscn_format_plugin.hpp"

#include <new>  // nothrow

#include "tactile/runtime/logging.hpp"

namespace tactile {

void GodotTscnFormatPlugin::load(IRuntime* runtime)
{
  log(LogLevel::kTrace, "Loading Godot TSCN format plugin");
  mRuntime = runtime;
}

void GodotTscnFormatPlugin::unload()
{
  log(LogLevel::kTrace, "Unloading Godot TSCN format plugin");
  mRuntime = nullptr;
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) GodotTscnFormatPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
