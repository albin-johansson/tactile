// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/engine/engine_app.hpp"

namespace tactile {

class IWindow;

/**
 * Represents the Tactile editor application.
 */
class TactileEditor final : public IEngineApp {
 public:
  explicit TactileEditor(IWindow* window);

  void on_startup() override;

  void on_shutdown() override;

  void on_update() override;

  void on_framebuffer_scale_changed(float framebuffer_scale) override;

 private:
  IWindow* m_window;
};

}  // namespace tactile
