#pragma once

#include "aliases/not_null.hpp"
#include "aliases/shared.hpp"
#include "aliases/tileset_id.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class MapDocument;
class Tileset;

class AddTilesetCmd final : public ACommand
{
 public:
  AddTilesetCmd(NotNull<MapDocument*> document,
                tileset_id id,
                Shared<Tileset> tileset);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddTileset;
  }

 private:
  MapDocument* mDocument{};
  tileset_id mId;
  Shared<Tileset> mTileset;
};

/// \} End of group core

}  // namespace Tactile
