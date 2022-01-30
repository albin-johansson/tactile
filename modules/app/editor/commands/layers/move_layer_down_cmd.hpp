#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class MoveLayerDownCmd final : public ACommand {
 public:
  MoveLayerDownCmd(RegistryRef registry, layer_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::MoveLayerDown;
  }

 private:
  RegistryRef mRegistry;
  layer_id mLayerId;
};

}  // namespace tactile
