#pragma once

#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"

namespace Tactile {

class MoveLayerUpCmd final : public MapCommand
{
 public:
  MoveLayerUpCmd(NotNull<MapDocument*> document, layer_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::MoveLayerUp;
  }

 private:
  layer_id mId;
};

}  // namespace Tactile
