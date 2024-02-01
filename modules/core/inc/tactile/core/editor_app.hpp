// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/engine/engine_app.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/dock_space.hpp"
#include "tactile/core/ui/editor_dock_widget.hpp"
#include "tactile/core/ui/layer_dock_widget.hpp"
#include "tactile/core/ui/menu_bar.hpp"
#include "tactile/core/ui/widget_manager.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/renderer.hpp"
#include "tactile/foundation/render/window.hpp"

namespace tactile {

using WidgetManagerType =
    WidgetManager<DockSpace, MenuBar, EditorDockWidget, LayerDockWidget>;

/**
 * Represents the Tactile editor application.
 */
class TACTILE_CORE_API EditorApp final : public IEngineApp {
 public:
  TACTILE_DELETE_COPY(EditorApp);
  TACTILE_DEFAULT_MOVE(EditorApp);

  EditorApp(IWindow* window, IRenderer* renderer);

  ~EditorApp() noexcept override;

  void on_startup() override;

  void on_shutdown() override;

  void on_update() override;

  void on_framebuffer_scale_changed(float framebuffer_scale) override;

 private:
  IWindow* mWindow {};
  IRenderer* mRenderer {};
  Unique<Model> mModel;
  EventDispatcher mEventDispatcher;
  WidgetManagerType mWidgets {};
};

}  // namespace tactile
