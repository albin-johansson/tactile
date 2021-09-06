#pragma once

#include <entt.hpp>  // registry

#include "aliases/layer_id.hpp"
#include "aliases/ref.hpp"
#include "aliases/tile_cache.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"

namespace Tactile {

/// \addtogroup commands
/// \{

class StampSequenceCmd final : public ACommand
{
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
