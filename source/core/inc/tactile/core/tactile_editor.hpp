// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/engine/engine_app.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/model/settings.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile {

class IWindow;
class IRenderer;

/**
 * Represents the Tactile editor application.
 */
class TactileEditor final : public IEngineApp
{
 public:
  TACTILE_DELETE_COPY(TactileEditor);
  TACTILE_DECLARE_MOVE(TactileEditor);

  TactileEditor(IWindow* window, IRenderer* renderer);

  ~TactileEditor() noexcept override;

  void on_startup() override;

  void on_shutdown() override;

  void on_update() override;

  void on_framebuffer_scale_changed(float framebuffer_scale) override;

 private:
  IWindow* mWindow;
  IRenderer* mRenderer;
  Settings mSettings {};
  Maybe<ui::Language> mLanguage {};
  Maybe<Model> mModel {};
  EventDispatcher mEventDispatcher {};
};

}  // namespace tactile
