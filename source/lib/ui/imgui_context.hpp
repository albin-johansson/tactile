// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/// Handles the configuration of the Dear ImGui context.
class ImGuiContext final
{
 public:
  TACTILE_DEFAULT_COPY(ImGuiContext);
  TACTILE_DEFAULT_MOVE(ImGuiContext);

  ImGuiContext();

  /// Reloads the main and icon fonts.
  ///
  /// This is meant to be called every time the application has reason to believe
  /// that the fonts need to be reloaded, e.g. when the reported display framebuffer size
  /// changes.
  static void reload_fonts();

 private:
  /// This is used as a workaround to annoying unit test issues for Windows CI builds
  bool mInitializedBackend {};
};

}  // namespace tactile
