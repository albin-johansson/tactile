// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/tiled_xml_format_plugin.hpp"

#include "tactile/foundation/io/save/save_format_context.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

void TiledXmlFormatPlugin::on_load()
{
  TACTILE_LOG_DEBUG("Loading Tiled XML format plugin");

  mFormat = make_unique<TiledXmlFormat>();
  SaveFormatContext::get().add_format(mFormat.get());
}

void TiledXmlFormatPlugin::on_unload() noexcept
{
  TACTILE_LOG_DEBUG("Unloading Tiled XML format plugin");

  SaveFormatContext::get().remove_format(mFormat.get());
  mFormat.reset();
}

auto tactile_create_plugin() -> IPlugin*
{
  return new TiledXmlFormatPlugin {};
}

void tactile_destroy_plugin(IPlugin* plugin)
{
  delete plugin;
}

auto tactile_plugin_id() -> const char*
{
  return "tactile.tiled-xml-format";
}

}  // namespace tactile
