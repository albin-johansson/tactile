// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/yaml_format/yaml_format_plugin.hpp"

#include <new>  // nothrow

#include "tactile/runtime/logging.hpp"

namespace tactile {

void YamlFormatPlugin::load(IRuntime* runtime)
{
  log(LogLevel::kTrace, "Loading Tactile YAML format plugin");
  mRuntime = runtime;
}

void YamlFormatPlugin::unload()
{
  log(LogLevel::kTrace, "Unloading Tactile YAML format plugin");
  mRuntime = nullptr;
}

auto tactile_make_plugin() -> IPlugin*
{
  return new (std::nothrow) YamlFormatPlugin {};
}

void tactile_free_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
