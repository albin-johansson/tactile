// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/plugin/plugin.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"
#include "tactile/tiled-xml-format/tiled_xml_format.hpp"

namespace tactile {

class TACTILE_TMX_FORMAT_API TiledXmlFormatPlugin final : public IPlugin {
 public:
  void on_load() override;

  void on_unload() noexcept override;

 private:
  Unique<TiledXmlFormat> mFormat;
};

extern "C"
{
  [[nodiscard]]
  TACTILE_TMX_FORMAT_API auto tactile_create_plugin() -> IPlugin*;

  TACTILE_TMX_FORMAT_API void tactile_destroy_plugin(IPlugin* plugin);

  [[nodiscard]]
  TACTILE_TMX_FORMAT_API auto tactile_plugin_id() -> const char*;
}

}  // namespace tactile
