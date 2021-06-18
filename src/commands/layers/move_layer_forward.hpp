#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "fwd.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

class MoveLayerForward final : public QUndoCommand
{
 public:
  MoveLayerForward(NotNull<core::MapDocument*> document, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::MoveLayerForward);
  }

 private:
  core::MapDocument* mDocument{};
  layer_id mId;
};

}  // namespace tactile::cmd
