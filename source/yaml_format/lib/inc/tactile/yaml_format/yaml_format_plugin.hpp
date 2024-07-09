// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/runtime/plugin.hpp"
#include "tactile/yaml_format/api.hpp"

namespace tactile {

class TACTILE_YAML_FORMAT_API YamlFormatPlugin final : public IPlugin
{
 public:
  void load(Runtime& runtime) override;

  void unload(Runtime& runtime) override;

 private:
};

extern "C"
{
  TACTILE_YAML_FORMAT_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_YAML_FORMAT_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
