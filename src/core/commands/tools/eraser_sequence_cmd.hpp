#pragma once

#include <vector_map.hpp>  // vector_map

#include "aliases/not_null.hpp"
#include "aliases/tile_id.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/map/map_position.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class MapDocument;

class EraserSequenceCmd final : public ACommand
{
 public:
  using TileCache = rune::vector_map<MapPosition, tile_id>;

  EraserSequenceCmd(NotNull<MapDocument*> document, TileCache&& oldState);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::EraserSequence;
  }

 private:
  MapDocument* mDocument{};
  TileCache mOldState;
};

/// \} End of group core

}  // namespace Tactile
