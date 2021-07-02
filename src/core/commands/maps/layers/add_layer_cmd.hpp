#pragma once

#include "aliases/layer_id.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"
#include "core/map/layers/layer.hpp"

namespace Tactile {

class AddLayerCmd final : public MapCommand
{
 public:
  AddLayerCmd(NotNull<MapDocument*> document, SharedLayer layer, layer_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddLayer;
  }

 private:
  SharedLayer mLayer;
  layer_id mId;
};

}  // namespace Tactile
