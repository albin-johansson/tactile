// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/event/view_event_handler.hpp"

#include <imgui.h>

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/view_events.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/model/settings.hpp"
#include "tactile/core/ui/widget_manager.hpp"

namespace tactile {

ViewEventHandler::ViewEventHandler(Model* model,
                                   IRenderer* renderer,
                                   ui::WidgetManager* widget_manager)
  : mModel {require_not_null(model, "null model")},
    mRenderer {require_not_null(renderer, "null renderer")},
    mWidgetManager {require_not_null(widget_manager, "null widget manager")}
{}

void ViewEventHandler::install(EventDispatcher& dispatcher)
{
  TACTILE_LOG_DEBUG("Installing view event handler");

  using Self = ViewEventHandler;

  // clang-format off
  dispatcher.bind<ResetLayoutEvent, &Self::on_reset_layout>(this);
  dispatcher.bind<TogglePropertyDockEvent, &Self::on_toggle_property_dock>(this);
  dispatcher.bind<ToggleComponentDockEvent, &Self::on_toggle_component_dock>(this);
  dispatcher.bind<ToggleLayerDockEvent, &Self::on_toggle_layer_dock>(this);
  dispatcher.bind<ToggleTilesetDockEvent, &Self::on_toggle_tileset_dock>(this);
  dispatcher.bind<ToggleAnimationDockEvent, &Self::on_toggle_animation_dock>(this);
  dispatcher.bind<ToggleLogDockEvent, &Self::on_toggle_log_dock>(this);
  dispatcher.bind<SetThemeEvent, &Self::on_set_theme>(this);
  dispatcher.bind<IncreaseFontSizeEvent, &Self::on_increase_font_size>(this);
  dispatcher.bind<DecreaseFontSizeEvent, &Self::on_decrease_font_size>(this);
  dispatcher.bind<ResetFontSizeEvent, &Self::on_reset_font_size>(this);
  dispatcher.bind<ReloadFontsEvent, &Self::on_reload_fonts>(this);
  dispatcher.bind<SetFontEvent, &Self::on_set_font>(this);
  dispatcher.bind<ToggleGridEvent, &Self::on_toggle_grid>(this);
  dispatcher.bind<ToggleLayerHighlightEvent, &Self::on_toggle_layer_highlight>(this);
  // clang-format on
}

void ViewEventHandler::on_reset_layout(const ResetLayoutEvent&)
{
  TACTILE_LOG_TRACE("ResetLayoutEvent");

  auto& dock_space = mWidgetManager->get_dock_space();
  dock_space.reset_layout(mModel->get_language());
}

void ViewEventHandler::on_toggle_property_dock(const TogglePropertyDockEvent&)
{
  TACTILE_LOG_TRACE("TogglePropertyDockEvent");
}

void ViewEventHandler::on_toggle_component_dock(const ToggleComponentDockEvent&)
{
  TACTILE_LOG_TRACE("ToggleComponentDockEvent");
}

void ViewEventHandler::on_toggle_layer_dock(const ToggleLayerDockEvent&)
{
  TACTILE_LOG_TRACE("ToggleLayerDockEvent");
}

void ViewEventHandler::on_toggle_tileset_dock(const ToggleTilesetDockEvent&)
{
  TACTILE_LOG_TRACE("ToggleTilesetDockEvent");
}

void ViewEventHandler::on_toggle_animation_dock(const ToggleAnimationDockEvent&)
{
  TACTILE_LOG_TRACE("ToggleAnimationDockEvent");
}

void ViewEventHandler::on_toggle_log_dock(const ToggleLogDockEvent&)
{
  TACTILE_LOG_TRACE("ToggleLogDockEvent");
}

void ViewEventHandler::on_set_theme(const SetThemeEvent&)
{
  TACTILE_LOG_TRACE("SetThemeEvent");
}

void ViewEventHandler::on_increase_font_size(const IncreaseFontSizeEvent&)
{
  TACTILE_LOG_TRACE("IncreaseFontSizeEvent");
}

void ViewEventHandler::on_decrease_font_size(const DecreaseFontSizeEvent&)
{
  TACTILE_LOG_TRACE("DecreaseFontSizeEvent");
}

void ViewEventHandler::on_reset_font_size(const ResetFontSizeEvent&)
{
  TACTILE_LOG_TRACE("ResetFontSizeEvent");
}

void ViewEventHandler::on_set_font(const SetFontEvent& event)
{
  TACTILE_LOG_TRACE("SetFontEvent");

  auto& settings = mModel->get_settings();
  settings.font = event.font;

  const auto& io = ImGui::GetIO();
  on_reload_fonts(ReloadFontsEvent {
    .framebuffer_scale = io.DisplayFramebufferScale.x,
  });
}

void ViewEventHandler::on_reload_fonts(const ReloadFontsEvent& event)
{
  TACTILE_LOG_TRACE("ReloadFontsEvent(framebuffer scale: {})", event.framebuffer_scale);
  ui::reload_fonts(*mRenderer, mModel->get_settings(), event.framebuffer_scale);
}

void ViewEventHandler::on_toggle_grid(const ToggleGridEvent&)
{
  TACTILE_LOG_TRACE("ToggleGridEvent");
}

void ViewEventHandler::on_toggle_layer_highlight(const ToggleLayerHighlightEvent&)
{
  TACTILE_LOG_TRACE("ToggleLayerHighlightEvent");
}

}  // namespace tactile
