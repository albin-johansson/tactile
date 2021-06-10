#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class MoveLayerBack final : public QUndoCommand
{
 public:
  MoveLayerBack(not_null<core::map_document*> document, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::move_layer_back);
  }

 private:
  core::map_document* mDocument{};
  layer_id mId;
};

}  // namespace tactile::cmd
