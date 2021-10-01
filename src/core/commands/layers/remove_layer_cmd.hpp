#pragma once

#include <entt.hpp>  // registry

#include "common/layer_id.hpp"
#include "common/maybe.hpp"
#include "common/ref.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/systems/layers/layer_snapshot.hpp"

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
