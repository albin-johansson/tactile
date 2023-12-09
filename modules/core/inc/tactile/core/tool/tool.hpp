// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/tool/mouse_event.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \interface ITool
 * \brief Interface for editor tool implementations.
 */
class ITool {
 public:
  TACTILE_INTERFACE_CLASS(ITool);

  /**
   * \brief Called whenever the tool is enabled (activated).
   */
  virtual void on_enable() = 0;

  /**
   * \brief Called whenever the tool is disabled (deactivated).
   *
   * \details This function is always called before a tool switch happens. The same is
   *          not guaranteed for other functions. For example, `on_mouse_pressed` is not
   *          necessarily followed by a `on_mouse_released` call. Therefore, tool
   *          implementations should reset any in-flight state in this function.
   */
  virtual void on_disable() = 0;

  /**
   * \brief Called whenever the mouse is pressed in the document.
   *
   * \param event the mouse event information.
   */
  virtual void on_mouse_pressed(const MouseEvent& event) = 0;

  /**
   * \brief Called whenever the mouse is dragged (while being pressed) in the document.
   *
   * \param event the mouse event information.
   */
  virtual void on_mouse_dragged(const MouseEvent& event) = 0;

  /**
   * \brief Called whenever the mouse is released in the document.
   *
   * \param event the mouse event information.
   */
  virtual void on_mouse_released(const MouseEvent& event) = 0;

  /**
   * \brief Indicates whether the tool is currently usable.
   *
   * \return true if the tool can be used; false otherwise.
   */
  [[nodiscard]]
  virtual auto is_usable() const -> bool = 0;
};

}  // namespace tactile
