#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class MoveLayerDownCmd final : public command_base {
 public:
  MoveLayerDownCmd(registry_ref registry, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::move_layer_down;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayerId;
};

}  // namespace tactile
