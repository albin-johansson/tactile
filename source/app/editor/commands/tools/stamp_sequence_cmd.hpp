#pragma once

#include <entt/entt.hpp>  // registry

#include "core/tile_cache.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class StampSequenceCmd final : public command_base {
 public:
  StampSequenceCmd(registry_ref registry, TileCache&& oldState, TileCache&& newState);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::stamp_sequence;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayer;
  TileCache mOldState;
  TileCache mNewState;

  void ApplySequence(const TileCache& cache);
};

/// \} End of group commands

}  // namespace tactile
