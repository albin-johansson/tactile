#pragma once

#include "aliases/maybe.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"

namespace Tactile {

class DuplicateLayerCmd final : public MapCommand
{
 public:
  DuplicateLayerCmd(NotNull<MapDocument*> document, layer_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::DuplicateLayer;
  }

 private:
  layer_id mId;
  Maybe<layer_id> mNewId;
};

}  // namespace Tactile
