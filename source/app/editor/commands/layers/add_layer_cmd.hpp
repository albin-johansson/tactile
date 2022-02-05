#pragma once

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class add_layer_cmd final : public command_base {
 public:
  add_layer_cmd(registry_ref registry, layer_type type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override { return command_id::add_layer; }

 private:
  registry_ref mRegistry;
  layer_type mLayerType;
  maybe<layer_id> mLayerId;
  maybe<sys::LayerSnapshot> mLayerSnapshot;
};

/// \} End of group commands

}  // namespace tactile
