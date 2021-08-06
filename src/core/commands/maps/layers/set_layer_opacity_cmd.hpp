#pragma once

#include "aliases/maybe.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

/// \addtogroup commands
/// \{

class SetLayerOpacityCmd final : public MapCommand
{
 public:
  SetLayerOpacityCmd(NotNull<MapDocument*> document, layer_id id, float opacity);

  void Undo() override;

  void Redo() override;

  auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetLayerOpacity;
  }

 private:
  layer_id mId;
  float mOpacity;
  Maybe<float> mPrevious;
};

/// \} End of group commands

/// \} End of group core

}  // namespace Tactile
