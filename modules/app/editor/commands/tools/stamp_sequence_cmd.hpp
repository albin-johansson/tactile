#pragma once

#include <entt/entt.hpp>  // registry

#include "core/tile_cache.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class StampSequenceCmd final : public ACommand {
 public:
  StampSequenceCmd(RegistryRef registry, TileCache&& oldState, TileCache&& newState);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::StampSequence;
  }

 private:
  RegistryRef mRegistry;
  layer_id mLayer;
  TileCache mOldState;
  TileCache mNewState;

  void ApplySequence(const TileCache& cache);
};

/// \} End of group commands

}  // namespace tactile
