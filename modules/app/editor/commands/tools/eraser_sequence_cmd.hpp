#pragma once

#include <entt/entt.hpp>  // registry

#include "core/tile_cache.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class EraserSequenceCmd final : public ACommand {
 public:
  EraserSequenceCmd(RegistryRef registry, TileCache&& oldState);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::EraserSequence;
  }

 private:
  RegistryRef mRegistry;
  layer_id mLayer;
  TileCache mOldState;
};

/// \} End of group commands

}  // namespace tactile