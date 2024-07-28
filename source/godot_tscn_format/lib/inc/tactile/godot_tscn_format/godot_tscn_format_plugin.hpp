// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/godot_tscn_format/api.hpp"
#include "tactile/runtime/plugin.hpp"

namespace tactile {

class TACTILE_TSCN_FORMAT_API GodotTscnFormatPlugin final : public IPlugin
{
 public:
  void load(IRuntime* runtime) override;

  void unload() override;

 private:
  IRuntime* mRuntime {};
};

extern "C"
{
  TACTILE_TSCN_FORMAT_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_TSCN_FORMAT_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
