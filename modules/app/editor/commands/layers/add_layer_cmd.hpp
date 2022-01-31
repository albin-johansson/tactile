#pragma once

#include <entt/entt.hpp>  // registry

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class AddLayerCmd final : public command_base {
 public:
  AddLayerCmd(registry_ref registry, layer_type type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::add_layer;
  }

 private:
  registry_ref mRegistry;
  layer_type mLayerType;
  maybe<layer_id> mLayerId;
  maybe<sys::LayerSnapshot> mLayerSnapshot;
};

/// \} End of group commands

}  // namespace tactile
