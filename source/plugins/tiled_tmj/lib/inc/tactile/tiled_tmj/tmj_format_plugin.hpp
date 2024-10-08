// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <memory>  // unique_ptr

#include "tactile/base/prelude.hpp"
#include "tactile/runtime/plugin.hpp"
#include "tactile/tiled_tmj/api.hpp"
#include "tactile/tiled_tmj/tmj_save_format.hpp"

namespace tactile {

class TACTILE_TMJ_FORMAT_API TmjFormatPlugin final : public IPlugin
{
 public:
  void load(IRuntime* runtime) override;

  void unload() override;

 private:
  IRuntime* mRuntime {};
  std::unique_ptr<TmjSaveFormat> mFormat {};
};

extern "C"
{
  TACTILE_TMJ_FORMAT_API auto tactile_make_plugin() -> IPlugin*;
  TACTILE_TMJ_FORMAT_API void tactile_free_plugin(IPlugin* plugin);
}

}  // namespace tactile
