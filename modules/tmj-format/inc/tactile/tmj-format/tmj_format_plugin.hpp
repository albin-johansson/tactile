// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/format/file_format.hpp"
#include "tactile/core/plugin/plugin.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/tmj-format/api.hpp"

namespace tactile {

/**
 * \brief Manages the Tiled TMJ format plugin.
 */
class TmjFormatPlugin final : public IPlugin {
 public:
  TACTILE_TMJ_API void on_load() override;

  TACTILE_TMJ_API void on_unload() override;

 private:
  Unique<IFileFormat> mTmjFormat;
};

extern "C"
{
  [[nodiscard]]
  TACTILE_TMJ_API auto tactile_create_plugin() -> IPlugin*;

  TACTILE_TMJ_API void tactile_destroy_plugin(IPlugin* plugin);
}

}  // namespace tactile
