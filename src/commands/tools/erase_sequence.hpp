#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "tile_id.hpp"
#include "vector_map.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class EraseSequence final : public QUndoCommand
{
 public:
  EraseSequence(not_null<core::map_document*> document,
                vector_map<core::position, tile_id>&& oldState);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::eraser);
  }

 private:
  core::map_document* mDocument{};
  vector_map<core::position, tile_id> mOldState;
  layer_id mLayer;
  bool mFirst{true};
};

}  // namespace tactile::cmd
