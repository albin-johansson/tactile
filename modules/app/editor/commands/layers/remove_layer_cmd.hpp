#pragma once

#include <entt/entt.hpp>  // registry

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class RemoveLayerCmd final : public command_base {
 public:
  RemoveLayerCmd(registry_ref registry, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::RemoveLayer;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayerId;
  maybe<sys::LayerSnapshot> mLayerSnapshot;
};

/// \} End of group commands

}  // namespace tactile
