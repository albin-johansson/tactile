#pragma once

#include "abstract_tool.hpp"
#include "map_document.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "vector_map.hpp"

namespace tactile {

/**
 * \class stamp_tool
 *
 * \brief Represents a "stamp" tool, which applies the current tileset selection
 * to the tiles in the map.
 *
 * \since 0.1.0
 *
 * \headerfile stamp_tool.hpp
 */
class stamp_tool final : public abstract_tool
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
  explicit stamp_tool(core::model* model);

  ~stamp_tool() noexcept override = default;

  void pressed(QMouseEvent* event, const QPointF& mapPosition) override;

  void moved(QMouseEvent* event, const QPointF& mapPosition) override;

  void released(QMouseEvent* event, const QPointF& mapPosition) override;

  void exited(QEvent* event) override;

  void disable() override;

 private:
  vector_map<core::position, tile_id> m_oldState;
  vector_map<core::position, tile_id> m_sequence;

  void update_stamp_sequence(core::map_document& map,
                             const core::tileset& tileset,
                             const core::position& origin);
};

}  // namespace tactile
