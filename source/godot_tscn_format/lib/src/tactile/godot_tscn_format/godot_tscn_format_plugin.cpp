// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot_tscn_format/godot_tscn_format_plugin.hpp"

#include <new>  // nothrow

#include "tactile/runtime/runtime.hpp"

namespace tactile {

void GodotTscnFormatPlugin::load(IRuntime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Loading Godot TSCN format plugin");
}

void GodotTscnFormatPlugin::unload(IRuntime& runtime)
{
  Runtime::log(LogLevel::kTrace, "Unloading Godot TSCN format plugin");
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
