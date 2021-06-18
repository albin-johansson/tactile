#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "fwd.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

class SetLayerVisibility final : public QUndoCommand
{
 public:
  SetLayerVisibility(NotNull<core::MapDocument*> document,
                     layer_id id,
                     bool visible);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::SetLayerVisibility);
  }

 private:
  NotNull<core::MapDocument*> mDocument{};
  layer_id mId;
  bool mVisible;
};

}  // namespace tactile::cmd
