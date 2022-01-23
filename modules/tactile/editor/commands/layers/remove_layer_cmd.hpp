#pragma once

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class RemoveLayerCmd final : public ACommand {
 public:
  RemoveLayerCmd(RegistryRef registry, LayerID id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveLayer;
  }

 private:
  RegistryRef mRegistry;
  LayerID mLayerId;
  Maybe<Sys::LayerSnapshot> mLayerSnapshot;
};

/// \} End of group commands

}  // namespace tactile
