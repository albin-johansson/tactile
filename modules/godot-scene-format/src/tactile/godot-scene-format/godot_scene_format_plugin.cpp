// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot-scene-format/godot_scene_format_plugin.hpp"

#include "tactile/foundation/io/save/save_format_context.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/godot-scene-format/godot_scene_format.hpp"

namespace tactile {

void GodotSceneFormatPlugin::on_load()
{
  TACTILE_LOG_DEBUG("Loading Godot scene format plugin");

  mFormat = make_unique<GodotSceneFormat>();
  SaveFormatContext::get().add_format(mFormat.get());
}

void GodotSceneFormatPlugin::on_unload() noexcept
{
  TACTILE_LOG_DEBUG("Unloading Godot scene format plugin");

  SaveFormatContext::get().remove_format(mFormat.get());
  mFormat.reset();
}

auto tactile_create_plugin() -> IPlugin*
{
  return new GodotSceneFormatPlugin {};
}

void tactile_destroy_plugin(IPlugin* plugin)
{
  delete plugin;
}

auto tactile_plugin_id() -> const char*
{
  return "tactile.godot-scene-format";
}

}  // namespace tactile
