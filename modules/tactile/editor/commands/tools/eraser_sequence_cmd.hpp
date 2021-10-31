#pragma once

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry

#include "common/tile_cache.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

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
