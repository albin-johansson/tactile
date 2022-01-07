#pragma once

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/tile_cache.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

/// \addtogroup commands
/// \{

class StampSequenceCmd final : public ACommand {
 public:
  StampSequenceCmd(Ref<entt::registry> registry,
                   TileCache&& oldState,
                   TileCache&& newState);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::StampSequence;
  }

 private:
  Ref<entt::registry> mRegistry;
  LayerID mLayer;
  TileCache mOldState;
  TileCache mNewState;

  void ApplySequence(const TileCache& cache);
};

/// \} End of group commands

}  // namespace Tactile
