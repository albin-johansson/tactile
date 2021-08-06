#pragma once

#include "aliases/layer_id.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

/// \addtogroup commands
/// \{

class SetLayerVisibleCmd final : public MapCommand
{
 public:
  SetLayerVisibleCmd(NotNull<MapDocument*> document, layer_id id, bool visible);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetLayerVisible;
  }

 private:
  layer_id mId;
  bool mVisible;
};

/// \} End of group commands

/// \} End of group core

}  // namespace Tactile
