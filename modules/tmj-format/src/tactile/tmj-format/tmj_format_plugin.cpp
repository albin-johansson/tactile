// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

//#include "tactile/tmj-format/tmj_format_plugin.hpp"

#include "tactile/tmj-format/tmj_format_plugin.hpp"

#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/format/save_format_manager.hpp"
#include "tactile/core/plugin/plugin_manager.hpp"
#include "tactile/tmj-format/tmj_format.hpp"

namespace tactile {

void TmjFormatPlugin::on_load()
{
  TACTILE_LOG_DEBUG("Loading TMJ format plugin...");

  mTmjFormat = make_unique<TmjFormat>();
  SaveFormatManager::get().add_format(mTmjFormat.get());
}

void TmjFormatPlugin::on_unload()
{
  TACTILE_LOG_DEBUG("Unloading TMJ format plugin...");

  SaveFormatManager::get().remove_format(mTmjFormat.get());
  mTmjFormat.reset();
}

auto tactile_create_plugin() -> IPlugin*
{
  return new TmjFormatPlugin {};
}

void tactile_destroy_plugin(IPlugin* plugin)
{
  delete plugin;
}

}  // namespace tactile
