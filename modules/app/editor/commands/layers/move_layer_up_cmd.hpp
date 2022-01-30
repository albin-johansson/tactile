#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class MoveLayerUpCmd final : public ACommand {
 public:
  MoveLayerUpCmd(RegistryRef registry, layer_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::MoveLayerUp;
  }

 private:
  RegistryRef mRegistry;
  layer_id mLayerId;
};

}  // namespace tactile
