#pragma once

#include "forward_declare.hpp"
#include "mouse_tool.hpp"
#include "smart_pointers.hpp"
#include "tool_id.hpp"
#include "vector_map.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, Model)

namespace tactile {

class tool_model final
{
 public:
  /**
   * \brief Creates a `tool_model` instance.
   *
   * \param model a pointer to the associated model, cannot be null.
   *
   * \throws tactile_error if the supplied pointer is null.
   *
   * \since 0.1.0
   */
  explicit tool_model(core::Model* model);

  /**
   * \brief Selects the specified tool.
   *
   * \param id the ID associated with the tool that will be enabled.
   *
   * \since 0.1.0
   */
  void select(tool_id id);

  /**
   * \brief Handles a mouse pressed event.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \since 0.1.0
   */
  void pressed(QMouseEvent* event, const QPointF& mapPosition);

  /**
   * \brief Handles a mouse moved event.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \since 0.1.0
   */
  void moved(QMouseEvent* event, const QPointF& mapPosition);

  /**
   * \brief Handles a mouse released event.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \since 0.1.0
   */
  void released(QMouseEvent* event, const QPointF& mapPosition);

  /**
   * \brief Handles an "entered" event, which is when the mouse enters the map
   * widget.
   *
   * \param event the associated event.
   *
   * \since 0.1.0
   */
  void entered(QEvent* event);

  /**
   * \brief Handles an "exited" event, which is when the mouse exits the map
   * widget.
   *
   * \param event the associated event.
   *
   * \since 0.1.0
   */
  void exited(QEvent* event);

 private:
  AMouseTool* m_current{};
  vector_map<tool_id, unique<AMouseTool>> m_tools;

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
  void switch_to(AMouseTool* tool);
};

}  // namespace tactile
