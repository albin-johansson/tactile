#pragma once

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class MoveLayerUpCmd final : public ACommand
{
 public:
  MoveLayerUpCmd(Ref<entt::registry> registry, LayerID id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::MoveLayerUp;
  }

 private:
  Ref<entt::registry> mRegistry;
  LayerID mLayerId;
};

}  // namespace Tactile
