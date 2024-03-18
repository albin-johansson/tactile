// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/window.hpp>

#include "init/sdl_context.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/persist/protobuf_context.hpp"
#include "ui/imgui_context.hpp"

namespace tactile {

class AppInitializer final {
 public:
  TACTILE_DELETE_COPY(AppInitializer);
  TACTILE_DELETE_MOVE(AppInitializer);

  [[nodiscard]] AppInitializer();

  [[nodiscard]] auto get_window() -> cen::window&;

 private:
  Maybe<ProtobufContext> mProtobuf;
  Maybe<SDLContext> mSDL;
  Maybe<ImGuiContext> mImGui;
};

void on_terminate();

}  // namespace tactile
