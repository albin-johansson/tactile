#pragma once

#include <QUndoCommand>  // QUndoCommand
#include <vector>        // vector

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "tile_id.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

class BucketFill final : public QUndoCommand
{
 public:
  /**
   * \brief Creates a bucket fill command.
   *
   * \param document the associated map document.
   * \param position the origin position of the flood fill.
   * \param replacement the tile ID that will be used to replace target tiles.
   *
   * \since 0.1.0
   */
  explicit BucketFill(not_null<core::MapDocument*> document,
                      const core::Position& position,
                      tile_id replacement);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::bucket_fill);
  }

 private:
  core::MapDocument* mDocument{};
  core::Position mOrigin;
  tile_id mReplacement;
  tile_id mTarget{};
  layer_id mLayer{};
  std::vector<core::Position> mPositions;
};

}  // namespace tactile::cmd
