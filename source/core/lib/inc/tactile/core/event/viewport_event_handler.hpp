// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Model;
class EventDispatcher;

struct OffsetViewportEvent;
struct UpdateViewportSizeEvent;
struct UpdateViewportLimitsEvent;
struct IncreaseViewportZoomEvent;
struct DecreaseViewportZoomEvent;
struct ResetViewportZoomEvent;
struct CenterViewportEvent;
struct PanViewportUpEvent;
struct PanViewportDownEvent;
struct PanViewportLeftEvent;
struct PanViewportRightEvent;

/**
 * Handles events related to document viewports.
 */
class ViewportEventHandler final
{
 public:
  /**
   * Creates a viewport event handler.
   *
   * \param model The associated model, cannot be null.
   */
  explicit ViewportEventHandler(Model* model);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Offsets the position of a viewport.
   *
   * \param event The associated event.
   */
  void on_offset_viewport(const OffsetViewportEvent& event);

  /**
   * Updates the size of a viewport.
   *
   * \param event The associated event.
   */
  void on_update_viewport_size(const UpdateViewportSizeEvent& event);

  /**
   * Updates the limits of a viewport.
   *
   * \param event The associated event.
   */
  void on_update_viewport_limits(const UpdateViewportLimitsEvent& event);

  /**
   * Increases the zoom of a viewport.
   *
   * \param event The associated event.
   */
  void on_increase_viewport_zoom(const IncreaseViewportZoomEvent& event);

  /**
   * Decreases the zoom of a viewport.
   *
   * \param event The associated event.
   */
  void on_decrease_viewport_zoom(const DecreaseViewportZoomEvent& event);

  /**
   * Resets the zoom of a viewport.
   *
   * \param event The associated event.
   */
  void on_reset_viewport_zoom(const ResetViewportZoomEvent& event);

  /**
   * Centers a viewport over the underlying content.
   *
   * \param event The associated event.
   */
  void on_center_viewport(const CenterViewportEvent& event);

  /**
   * Pans a viewport upwards.
   *
   * \param event The associated event.
   */
  void on_pan_viewport_up(const PanViewportUpEvent& event);

  /**
   * Pans a viewport downwards.
   *
   * \param event The associated event.
   */
  void on_pan_viewport_down(const PanViewportDownEvent& event);

  /**
   * Pans a viewport to the left.
   *
   * \param event The associated event.
   */
  void on_pan_viewport_left(const PanViewportLeftEvent& event);

  /**
   * Pans a viewport to the right.
   *
   * \param event The associated event.
   */
  void on_pan_viewport_right(const PanViewportRightEvent& event);

 private:
  Model* mModel;

  void _pan_viewport(EntityID viewport_entity, const Float2& tile_offset_factor);
};

}  // namespace tactile
