#pragma once

#include <QMouseEvent>
#include <QPoint>

#include "maybe.hpp"
#include "position.hpp"

namespace tactile {
namespace core {
class model;
}

/**
 * \class abstract_tool
 *
 * \brief The ABC for mouse tool implementations.
 *
 * \since 0.1.0
 *
 * \headerfile abstract_tool.hpp
 */
class abstract_tool
{
 public:
  virtual ~abstract_tool() noexcept = default;

  /**
   * \brief Handles a mouse pressed event.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \note For convenience, the default implementation does nothing.
   *
   * \since 0.1.0
   */
  virtual void pressed(QMouseEvent* event, const QPointF& mapPosition){};

  /**
   * \brief Handles a mouse moved event.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \note For convenience, the default implementation does nothing.
   *
   * \since 0.1.0
   */
  virtual void moved(QMouseEvent* event, const QPointF& mapPosition){};

  /**
   * \brief Handles a mouse released event.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \note For convenience, the default implementation does nothing.
   *
   * \since 0.1.0
   */
  virtual void released(QMouseEvent* event, const QPointF& mapPosition){};

  /**
   * \brief Handles an "entered" event, which is when the mouse enters the map
   * widget.
   *
   * \param event the associated event.
   *
   * \note For convenience, the default implementation does nothing.
   *
   * \since 0.1.0
   */
  virtual void entered(QEvent* event){};

  /**
   * \brief Handles an "exited" event, which is when the mouse exits the map
   * widget.
   *
   * \param event the associated event.
   *
   * \note For convenience, the default implementation does nothing.
   *
   * \since 0.1.0
   */
  virtual void exited(QEvent* event){};

  /**
   * \brief Enables the mouse tool.
   *
   * \details This function should be called everytime the mouse tool is
   * selected.
   *
   * \note For convenience, the default implementation does nothing.
   *
   * \since 0.1.0
   */
  virtual void enable(){};

  /**
   * \brief Disables the mouse tool.
   *
   * \details This function should be called before switching to another tool.
   *
   * \note For convenience, the default implementation does nothing.
   *
   * \since 0.1.0
   */
  virtual void disable(){};

 protected:
  /**
   * \brief Creates an `abstract_tool` instance.
   *
   * \param model a pointer to the associated model, cannot be null.
   *
   * \throws tactile_error if the supplied pointer is null.
   *
   * \since 0.1.0
   */
  explicit abstract_tool(core::model* model);

  /**
   * \brief Returns a pointer to the associated model.
   *
   * \note The returned pointer will never be null.
   *
   * \return a pointer to the associated model.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto get_model() noexcept -> core::model*;

  /**
   * \copydoc get_model()
   */
  [[nodiscard]] auto get_model() const noexcept -> const core::model*;

  /**
   * \brief Translates a mouse position to a row/col map coordinate if the mouse
   * is within the map.
   *
   * \param mousePosition the current position of the mouse.
   * \param mapPosition the current position of the map.
   *
   * \return the mouse position translated as a map position, if the mouse is in
   * bounds of the map; `std::nullopt` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto translate_mouse_position(const QPoint& mousePosition,
                                              const QPointF& mapPosition) const
      -> maybe<core::position>;

 private:
  core::model* m_model{};
};

}  // namespace tactile
