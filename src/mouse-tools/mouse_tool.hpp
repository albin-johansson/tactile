#pragma once

#include <QMouseEvent>  // QMouseEvent
#include <QPoint>       // QPoint

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "position.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, Model)

namespace tactile {

/**
 * \class AMouseTool
 *
 * \brief The ABC for mouse tool implementations.
 *
 * \since 0.1.0
 *
 * \headerfile mouse_tool.hpp
 */
class AMouseTool
{
 public:
  virtual ~AMouseTool() noexcept = default;

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
  virtual void OnPressed(QMouseEvent* event, const QPointF& mapPosition)
  {}

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
  virtual void OnMoved(QMouseEvent* event, const QPointF& mapPosition)
  {}

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
  virtual void OnReleased(QMouseEvent* event, const QPointF& mapPosition)
  {}

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
  virtual void OnEntered(QEvent* event)
  {}

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
  virtual void OnExited(QEvent* event)
  {}

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
  virtual void Enable()
  {}

  /**
   * \brief Disables the mouse tool.
   *
   * \details This function should be called before switching to another tool.
   *
   * \note For convenience, the default implementation does nothing.
   *
   * \since 0.1.0
   */
  virtual void Disable()
  {}

 protected:
  /**
   * \brief Creates an `abstract_tool` instance.
   *
   * \param model a pointer to the associated model, cannot be null.
   *
   * \throws TactileError if the supplied pointer is null.
   *
   * \since 0.1.0
   */
  explicit AMouseTool(core::Model* model);

  /**
   * \brief Returns a pointer to the associated model.
   *
   * \note The returned pointer will never be null.
   *
   * \return a pointer to the associated model.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto GetModel() noexcept -> core::Model*;

  /// \copydoc GetModel()
  [[nodiscard]] auto GetModel() const noexcept -> const core::Model*;

  /**
   * \brief Translates a mouse position to a row/col map coordinate if the mouse
   * is within the map.
   *
   * \param mousePosition the current position of the mouse.
   * \param mapPosition the current position of the map.
   *
   * \return the mouse position translated as a map position, if the mouse is in
   * bounds of the map; `nothing` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto TranslateMousePosition(const QPoint& mousePosition,
                                            const QPointF& mapPosition) const
      -> Maybe<core::Position>;

 private:
  core::Model* mModel{};
};

}  // namespace tactile
