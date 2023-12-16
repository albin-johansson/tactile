// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-json-format/tiled_json_format_plugin.hpp"

#include "tactile/foundation/io/save/save_format_context.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tiled-json-format/tiled_json_format.hpp"

namespace tactile {

void TiledJsonFormatPlugin::on_load()
{
  TACTILE_LOG_DEBUG("Loading Tiled JSON format plugin");

  mTmjFormat = make_unique<TiledJsonFormat>();
  SaveFormatContext::get().add_format(mTmjFormat.get());
}

void TiledJsonFormatPlugin::on_unload() noexcept
{
  TACTILE_LOG_DEBUG("Unloading Tiled JSON format plugin");

  SaveFormatContext::get().remove_format(mTmjFormat.get());
  mTmjFormat.reset();
}

auto tactile_create_plugin() -> IPlugin*
{
  return new TiledJsonFormatPlugin {};
}

void tactile_destroy_plugin(IPlugin* plugin)
{
  delete plugin;
}

auto tactile_plugin_id() -> const char*
{
  return "tactile.tiled-json-format";
}

}  // namespace tactile
