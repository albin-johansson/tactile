// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>  // optional

#include "event/object_event_handler.hpp"
#include "tactile/base/engine/engine_app.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/render/renderer.hpp"
#include "tactile/base/render/window.hpp"
#include "tactile/base/runtime/runtime.hpp"
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

namespace tactile::core {

/**
 * Represents the Tactile editor application.
 */
class TactileApp final : public IEngineApp
{
 public:
  TACTILE_DELETE_COPY(TactileApp);
  TACTILE_DELETE_MOVE(TactileApp);

  explicit TactileApp(IRuntime* runtime);

  ~TactileApp() noexcept override;

  void on_startup() override;

  void on_shutdown() override;

  void on_update() override;

  void on_render() override;

  void on_framebuffer_scale_changed(float framebuffer_scale) override;

 private:
  IRuntime* m_runtime;
  IWindow* m_window;
  IRenderer* m_renderer;

  /** The user-controlled editor settings. */
  Settings m_settings;

  /** The language that will be used throughout the session. */
  std::optional<ui::Language> m_language;

  /** The core document model. */
  std::optional<Model> m_model;

  /** The UI manager. */
  ui::WidgetManager m_widget_manager;

  /** The lone event dispatcher used to track events. */
  EventDispatcher m_event_dispatcher;

  /** Delegate for general "file" events. */
  std::optional<FileEventHandler> m_file_event_handler;

  /** Delegate for general "edit" events. */
  std::optional<EditEventHandler> m_edit_event_handler;

  /** Delegate for general "view" events. */
  std::optional<ViewEventHandler> m_view_event_handler;

  /** Delegate for tileset events. */
  std::optional<TilesetEventHandler> m_tileset_event_handler;

  /** Delegate for map events. */
  std::optional<MapEventHandler> m_map_event_handler;

  /** Delegate for layer events. */
  std::optional<LayerEventHandler> m_layer_event_handler;

  /** Delegate for object events. */
  std::optional<ObjectEventHandler> m_object_event_handler;

  /** Delegate for component events. */
  std::optional<ComponentEventHandler> m_component_event_handler;

  /** Delegate for property events. */
  std::optional<PropertyEventHandler> m_property_event_handler;

  /** Delegate for viewport events. */
  std::optional<ViewportEventHandler> m_viewport_event_handler;
};

}  // namespace tactile::core
