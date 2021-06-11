#pragma once

#include "map_document.hpp"
#include "mouse_tool.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "vector_map.hpp"

namespace tactile {

/**
 * \class StampTool
 *
 * \brief Represents a "stamp" tool, which applies the current tileset selection
 * to the tiles in the map.
 *
 * \since 0.1.0
 *
 * \headerfile stamp_tool.hpp
 */
class StampTool final : public AMouseTool
{
 public:
  /**
   * \brief Creates an `stamp_tool` instance.
   *
   * \param model a pointer to the associated model, cannot be null.
   *
   * \throws tactile_error if the supplied pointer is null.
   *
   * \since 0.1.0
   */
  explicit StampTool(core::Model* model);

  ~StampTool() noexcept override = default;

  void OnPressed(QMouseEvent* event, const QPointF& mapPosition) override;

  void OnMoved(QMouseEvent* event, const QPointF& mapPosition) override;

  void OnReleased(QMouseEvent* event, const QPointF& mapPosition) override;

  void OnExited(QEvent* event) override;

  void Disable() override;

 private:
  vector_map<core::Position, tile_id> mOldState;
  vector_map<core::Position, tile_id> mSequence;

  void update_stamp_sequence(core::MapDocument& map,
                             const core::Tileset& tileset,
                             const core::Position& origin);
};

}  // namespace tactile
