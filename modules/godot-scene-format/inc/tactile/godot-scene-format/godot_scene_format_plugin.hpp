// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/io/save/save_format.hpp"
#include "tactile/foundation/plugin/plugin.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/godot-scene-format/api.hpp"

namespace tactile {

class TACTILE_TSCN_FORMAT_API GodotSceneFormatPlugin final : public IPlugin {
 public:
  void on_load() override;

  void on_unload() noexcept override;

 private:
  Unique<ISaveFormat> mFormat {};
};

extern "C"
{
  [[nodiscard]]
  TACTILE_TSCN_FORMAT_API auto tactile_create_plugin() -> IPlugin*;

  TACTILE_TSCN_FORMAT_API void tactile_destroy_plugin(IPlugin* plugin);

  [[nodiscard]]
  TACTILE_TSCN_FORMAT_API auto tactile_plugin_id() -> const char*;
}

}  // namespace tactile
