#pragma once

#include <entt.hpp>  // registry

#include <tactile-base/tactile_std.hpp>

#include "core/systems/layers/layer_snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

/// \addtogroup commands
/// \{

class RemoveLayerCmd final : public ACommand
{
 public:
  RemoveLayerCmd(Ref<entt::registry> registry, LayerID id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveLayer;
  }

 private:
  Ref<entt::registry> mRegistry;
  LayerID mLayerId;
  Maybe<Sys::LayerSnapshot> mLayerSnapshot;
};

/// \} End of group commands

}  // namespace Tactile
