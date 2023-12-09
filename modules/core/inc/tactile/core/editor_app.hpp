// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/engine/engine_app.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/renderer.hpp"
#include "tactile/foundation/render/window.hpp"

namespace tactile {

/**
 * \brief Represents the Tactile editor application.
 */
class TACTILE_CORE_API EditorApp final : public IEngineApp {
 public:
  EditorApp(IWindow* window, IRenderer* renderer);

  void on_startup() override;

  void on_shutdown() override;

  void on_update() override;

  void on_framebuffer_scale_changed(float framebuffer_scale) override;

 private:
  IWindow* mWindow {};
  IRenderer* mRenderer {};
};

}  // namespace tactile
