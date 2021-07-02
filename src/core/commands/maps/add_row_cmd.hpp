#pragma once

#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"

namespace Tactile {

class MapDocument;

class AddRowCmd final : public MapCommand
{
 public:
  explicit AddRowCmd(NotNull<MapDocument*> document);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddRow;
  }
};

}  // namespace Tactile
