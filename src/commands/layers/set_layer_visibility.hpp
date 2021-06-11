#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

class SetLayerVisibility final : public QUndoCommand
{
 public:
  SetLayerVisibility(not_null<core::MapDocument*> document,
                     layer_id id,
                     bool visible);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::SetLayerVisibility);
  }

 private:
  not_null<core::MapDocument*> mDocument{};
  layer_id mId;
  bool mVisible;
};

}  // namespace tactile::cmd
