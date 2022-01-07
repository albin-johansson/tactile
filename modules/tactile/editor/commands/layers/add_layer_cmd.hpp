#pragma once

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

/// \addtogroup commands
/// \{

class AddLayerCmd final : public ACommand {
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
