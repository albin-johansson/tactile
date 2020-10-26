#pragma once

#include <memory>  // unique_ptr

#include "abstract_tool.hpp"
#include "tool_id.hpp"

namespace tactile {
namespace core {
class model;
}

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
  explicit tool_model(core::model* model);

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
  abstract_tool* m_current{};
  std::unique_ptr<abstract_tool> m_stamp;
  std::unique_ptr<abstract_tool> m_eraser;
  std::unique_ptr<abstract_tool> m_bucket;

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
  void switch_to(abstract_tool* tool);
};

}  // namespace tactile
