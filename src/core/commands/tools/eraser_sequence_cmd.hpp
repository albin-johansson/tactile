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

class EraserSequenceCmd final : public ACommand
{
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
