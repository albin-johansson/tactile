// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/tactile_yml_format_plugin.hpp"

#include "tactile/foundation/log/logger.hpp"

namespace tactile {

void TactileYmlFormatPlugin::on_load()
{
  TACTILE_LOG_DEBUG("Loading Tactile YML format plugin...");

  mFormat = make_unique<TactileYmlFormat>();
}

void TactileYmlFormatPlugin::on_unload() noexcept
{
  TACTILE_LOG_DEBUG("Unloading Tactile YML format plugin...");

  mFormat.reset();
}

auto tactile_create_plugin() -> IPlugin*
{
  return new TactileYmlFormatPlugin {};
}

void tactile_destroy_plugin(IPlugin* plugin)
{
  delete plugin;
}

auto tactile_plugin_id() -> const char*
{
  return "tactile.tactile-yml-format";
}

}  // namespace tactile
