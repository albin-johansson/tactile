// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/engine/engine_app.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/event/component_event_handler.hpp"
#include "tactile/core/event/edit_event_handler.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/file_event_handler.hpp"
#include "tactile/core/event/layer_event_handler.hpp"
#include "tactile/core/event/map_event_handler.hpp"
#include "tactile/core/event/property_event_handler.hpp"
#include "tactile/core/event/tileset_event_handler.hpp"
#include "tactile/core/event/view_event_handler.hpp"
#include "tactile/core/event/viewport_event_handler.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/model/settings.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/widget_manager.hpp"

namespace tactile {

class IWindow;
class IRenderer;

/**
 * Represents the Tactile editor application.
 */
class TactileApp final : public IEngineApp
{
 public:
  TACTILE_DELETE_COPY(TactileApp);
  TACTILE_DELETE_MOVE(TactileApp);

  TactileApp(IWindow* window, IRenderer* renderer);

  ~TactileApp() noexcept override;

  void on_startup() override;

  void on_shutdown() override;

  void on_update() override;

  void on_render() override;

  void on_framebuffer_scale_changed(float framebuffer_scale) override;

 private:
  IWindow* mWindow;
  IRenderer* mRenderer;

  /** The user-controlled editor settings. */
  Settings mSettings {};

  /** The language that will be used throughout the session. */
  Optional<ui::Language> mLanguage {};

  /** The core document model. */
  Optional<Model> mModel {};

  /** The UI manager. */
  ui::WidgetManager mWidgetManager {};

  /** The lone event dispatcher used to track events. */
  EventDispatcher mEventDispatcher {};

  /** Delegate for general "file" events. */
  Optional<FileEventHandler> mFileEventHandler {};

  /** Delegate for general "edit" events. */
  Optional<EditEventHandler> mEditEventHandler {};

  /** Delegate for general "view" events. */
  Optional<ViewEventHandler> mViewEventHandler {};

  /** Delegate for tileset events. */
  Optional<TilesetEventHandler> mTilesetEventHandler {};

  /** Delegate for map events. */
  Optional<MapEventHandler> mMapEventHandler {};

  /** Delegate for layer events. */
  Optional<LayerEventHandler> mLayerEventHandler {};

  /** Delegate for component events. */
  Optional<ComponentEventHandler> mComponentEventHandler {};

  /** Delegate for property events. */
  Optional<PropertyEventHandler> mPropertyEventHandler {};

  /** Delegate for viewport events. */
  Optional<ViewportEventHandler> mViewportEventHandler {};
};

}  // namespace tactile
