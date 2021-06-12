#pragma once

#include "map_position.hpp"
#include "mouse_tool.hpp"
#include "tile_id.hpp"
#include "vector_map.hpp"

namespace tactile {

/**
 * \class EraserTool
 *
 * \brief Represents an "eraser" tool, which sets tiles to be empty.
 *
 * \since 0.1.0
 *
 * \headerfile eraser_tool.hpp
 */
class EraserTool final : public AMouseTool
{
 public:
  /**
   * \brief Creates an eraser tool.
   *
   * \param model a pointer to the associated model, cannot be null.
   *
   * \throws TactileError if the supplied pointer is null.
   *
   * \since 0.1.0
   */
  explicit EraserTool(core::Model* model);

  ~EraserTool() noexcept override = default;

  void OnPressed(QMouseEvent* event, const QPointF& mapPosition) override;

  void OnMoved(QMouseEvent* event, const QPointF& mapPosition) override;

  void OnReleased(QMouseEvent* event, const QPointF& mapPosition) override;

 private:
  vector_map<core::MapPosition, tile_id> mOldState;

  /**
   * \brief Updates the state of the eraser, by erasing the tile at the mouse
   * position.
   *
   * \details This function is meant to be called from the `OnPressed` and
   * `OnMoved` functions.
   *
   * \param event the associated event.
   * \param mapPosition the current position of the map.
   *
   * \since 0.1.0
   */
  void UpdateEraser(QMouseEvent* event, const QPointF& mapPosition);
};

}  // namespace tactile
