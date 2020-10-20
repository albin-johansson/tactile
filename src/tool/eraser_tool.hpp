#pragma once

#include "abstract_tool.hpp"
#include "position.hpp"
#include "tile_id.hpp"
#include "vector_map.hpp"

namespace tactile {

/**
 * @class eraser_tool
 *
 * @brief Represents an "eraser" tool, which sets tiles to be empty.
 *
 * @since 0.1.0
 *
 * @headerfile eraser_tool.hpp
 */
class eraser_tool final : public abstract_tool
{
 public:
  /**
   * @brief Creates an `eraser_tool` instance.
   *
   * @param model a pointer to the associated model, cannot be null.
   *
   * @throws tactile_error if the supplied pointer is null.
   *
   * @since 0.1.0
   */
  explicit eraser_tool(core::model* model);

  ~eraser_tool() noexcept override = default;

  void pressed(QMouseEvent* event, const QPointF& mapPosition) override;

  void moved(QMouseEvent* event, const QPointF& mapPosition) override;

  void released(QMouseEvent* event, const QPointF& mapPosition) override;

 private:
  vector_map<core::position, tile_id> m_oldState;

  /**
   * @brief Updates the state of the eraser, by erasing the tile at the mouse
   * position.
   *
   * @details This function is meant to be called from the `pressed` and `moved`
   * functions.
   *
   * @param event the associated event.
   * @param mapPosition the current position of the map.
   *
   * @since 0.1.0
   */
  void update_eraser(QMouseEvent* event, const QPointF& mapPosition);
};

}  // namespace tactile
