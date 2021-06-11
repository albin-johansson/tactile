#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "tile_id.hpp"
#include "vector_map.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

class StampSequence final : public QUndoCommand
{
 public:
  StampSequence(not_null<core::MapDocument*> document,
                vector_map<core::Position, tile_id>&& oldState,
                vector_map<core::Position, tile_id>&& sequence);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::Stamp);
  }

 private:
  core::MapDocument* mDocument{};
  vector_map<core::Position, tile_id> mOldState;
  vector_map<core::Position, tile_id> mSequence;
  layer_id mLayer;
  bool mFirst{true};
};

}  // namespace tactile::cmd
