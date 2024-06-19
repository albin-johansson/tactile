// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;
class EventDispatcher;

namespace ui {
class WidgetManager;
}  // namespace ui

struct ResetLayoutEvent;
struct TogglePropertyDockEvent;
struct ToggleComponentDockEvent;
struct ToggleLayerDockEvent;
struct ToggleTilesetDockEvent;
struct ToggleAnimationDockEvent;
struct ToggleLogDockEvent;
struct SetThemeEvent;
struct IncreaseFontSizeEvent;
struct DecreaseFontSizeEvent;
struct ResetFontSizeEvent;
struct ToggleGridEvent;
struct ToggleLayerHighlightEvent;

/**
 * Handles miscellaneous view related events.
 */
class ViewEventHandler
{
 public:
  /**
   * Creates a view event handler.
   *
   * \param model          The associated model, cannot be null.
   * \param widget_manager The associated widget manager, cannot be null.
   */
  ViewEventHandler(Model* model, ui::WidgetManager* widget_manager);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Resets the layout of the dock widgets.
   *
   * \param event The associated event.
   */
  void on_reset_layout(const ResetLayoutEvent& event);

  /**
   * Toggles the visibility of the property dock.
   *
   * \param event The associated event.
   */
  void on_toggle_property_dock(const TogglePropertyDockEvent& event);

  /**
   * Toggles the visibility of the component dock.
   *
   * \param event The associated event.
   */
  void on_toggle_component_dock(const ToggleComponentDockEvent& event);

  /**
   * Toggles the visibility of the layer dock.
   *
   * \param event The associated event.
   */
  void on_toggle_layer_dock(const ToggleLayerDockEvent& event);

  /**
   * Toggles the visibility of the tileset dock.
   *
   * \param event The associated event.
   */
  void on_toggle_tileset_dock(const ToggleTilesetDockEvent& event);

  /**
   * Toggles the visibility of the animation dock.
   *
   * \param event The associated event.
   */
  void on_toggle_animation_dock(const ToggleAnimationDockEvent& event);

  /**
   * Toggles the visibility of the log dock.
   *
   * \param event The associated event.
   */
  void on_toggle_log_dock(const ToggleLogDockEvent& event);

  /**
   * Changes the active editor theme.
   *
   * \param event The associated event.
   */
  void on_set_theme(const SetThemeEvent& event);

  /**
   * Increases the UI font size.
   *
   * \param event The associated event.
   */
  void on_increase_font_size(const IncreaseFontSizeEvent& event);

  /**
   * Decreases the UI font size.
   *
   * \param event The associated event.
   */
  void on_decrease_font_size(const DecreaseFontSizeEvent& event);

  /**
   * Resets the UI font size.
   *
   * \param event The associated event.
   */
  void on_reset_font_size(const ResetFontSizeEvent& event);

  /**
   * Toggles the visibility of the viewport grid.
   *
   * \param event The associated event.
   */
  void on_toggle_grid(const ToggleGridEvent& event);

  /**
   * Toggles the layer highlighting mode.
   *
   * \param event The associated event.
   */
  void on_toggle_layer_highlight(const ToggleLayerHighlightEvent& event);

 private:
  Model* mModel;
  ui::WidgetManager* mWidgetManager;
};

}  // namespace tactile
