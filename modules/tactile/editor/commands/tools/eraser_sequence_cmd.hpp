#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "core/tile_cache.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

/// \addtogroup commands
/// \{

class EraserSequenceCmd final : public ACommand {
 public:
  EraserSequenceCmd(Ref<entt::registry> registry, TileCache&& oldState);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::EraserSequence;
  }

 private:
  Ref<entt::registry> mRegistry;
  LayerID mLayer;
  TileCache mOldState;
};

/// \} End of group commands

}  // namespace Tactile
