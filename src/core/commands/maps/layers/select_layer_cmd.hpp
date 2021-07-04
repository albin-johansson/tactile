#pragma once

#include "aliases/layer_id.hpp"
#include "aliases/maybe.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"

namespace Tactile {

class SelectLayerCmd final : public MapCommand
{
 public:
  SelectLayerCmd(NotNull<MapDocument*> document, layer_id id);

  void Undo() override;

  void Redo() override;

  auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SelectLayer;
  }

 private:
  layer_id mId;
  Maybe<layer_id> mPrevious;
};

}  // namespace Tactile
