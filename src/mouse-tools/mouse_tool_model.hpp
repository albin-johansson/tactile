#pragma once

#include "forward_declare.hpp"
#include "mouse_tool.hpp"
#include "mouse_tool_type.hpp"
#include "smart_pointers.hpp"
#include "vector_map.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, Model)

namespace tactile {

class MouseToolModel final
{
 public:
  /**
   * \brief Creates a mouse tool model.
   *
   * \param model a pointer to the associated model, cannot be null.
   *
   * \throws TactileError if the supplied pointer is null.
   *
   * \since 0.1.0
   */
  explicit MouseToolModel(core::Model* model);

  /**
   * \brief Selects the specified tool.
   *
   * \param id the ID associated with the tool that will be enabled.
   *
   * \since 0.1.0
   */
  void Select(MouseToolType id);

  /**
   * \brief Handles a mouse OnMousePressed event.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \since 0.1.0
   */
  void OnMousePressed(QMouseEvent* event, const QPointF& mapPosition);

  /**
   * \brief Handles a mouse OnMouseMoved event.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \since 0.1.0
   */
  void OnMouseMoved(QMouseEvent* event, const QPointF& mapPosition);

  /**
   * \brief Handles a mouse OnMouseReleased event.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \since 0.1.0
   */
  void OnMouseReleased(QMouseEvent* event, const QPointF& mapPosition);

  /**
   * \brief Handles an "OnMouseEntered" event, which is when the mouse enters
   * the map widget.
   *
   * \param event the associated event.
   *
   * \since 0.1.0
   */
  void OnMouseEntered(QEvent* event);

  /**
   * \brief Handles an "OnMouseExited" event, which is when the mouse exits the
   * map widget.
   *
   * \param event the associated event.
   *
   * \since 0.1.0
   */
  void OnMouseExited(QEvent* event);

 private:
  AMouseTool* mCurrent{};
  vector_map<MouseToolType, Unique<AMouseTool>> mTools;

  /**
   * \brief Switches to the specified tool as the active tool.
   *
   * \note This function has no effect if the supplied tool is already the
   * active tool.
   *
   * \param tool a pointer to the tool that will be enabled; `nullptr` if no
   * tool should be enabled.
   *
   * \since 0.1.0
   */
  void SwitchTo(AMouseTool* tool);
};

}  // namespace tactile
