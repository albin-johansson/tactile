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

class RemoveTilesetCmd final : public ACommand
{
 public:
  RemoveTilesetCmd(NotNull<MapDocument*> document, tileset_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveTileset;
  }

 private:
  MapDocument* mDocument{};
  tileset_id mId;
  Shared<Tileset> mTileset;
};

/// \} End of group core

}  // namespace Tactile
