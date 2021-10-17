#pragma once

#include <entt.hpp>  // registry

#include "common/layer_id.hpp"
#include "common/maybe.hpp"
#include "common/ref.hpp"
#include "core/layer_type.hpp"
#include "core/systems/layers/layer_snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

/// \addtogroup commands
/// \{

class AddLayerCmd final : public ACommand
{
 public:
  AddLayerCmd(Ref<entt::registry> registry, LayerType type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddLayer;
  }

 private:
  Ref<entt::registry> mRegistry;
  LayerType mLayerType;
  Maybe<LayerID> mLayerId;
  Maybe<Sys::LayerSnapshot> mLayerSnapshot;
};

/// \} End of group commands

}  // namespace Tactile
