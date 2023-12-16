// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/io/save/save_format.hpp"
#include "tactile/foundation/plugin/plugin.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"

namespace tactile::tmj {

/**
 * \brief Manages the Tiled TMJ format plugin.
 */
class TACTILE_TMJ_API TmjFormatPlugin final : public IPlugin {
 public:
  void on_load() override;

  void on_unload() noexcept override;

 private:
  Unique<ISaveFormat> mTmjFormat;
};

extern "C"
{
  [[nodiscard]]
  TACTILE_TMJ_API auto tactile_create_plugin() -> IPlugin*;

  TACTILE_TMJ_API void tactile_destroy_plugin(IPlugin* plugin);

  [[nodiscard]]
  TACTILE_TMJ_API auto tactile_plugin_id() -> const char*;
}

}  // namespace tactile::tmj
