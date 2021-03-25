#pragma once

#include <QUndoCommand>  // QUndoCommand
#include <vector>        // vector

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "tile_id.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class bucket_fill final : public QUndoCommand
{
 public:
  /**
   * \brief Creates a `bucket_fill` instance.
   *
   * \param document the associated map document.
   * \param position the origin position of the flood fill.
   * \param replacement the tile ID that will be used to replace target tiles.
   *
   * \since 0.1.0
   */
  explicit bucket_fill(not_null<core::map_document*> document,
                       const core::position& position,
                       tile_id replacement);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::bucket_fill);
  }

 private:
  core::map_document* m_document{};
  core::position m_origin;
  tile_id m_replacement;
  tile_id m_target{};
  layer_id m_layer{};
  std::vector<core::position> m_positions;
};

}  // namespace tactile::cmd
