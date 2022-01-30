#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class DuplicateLayerCmd final : public ACommand {
 public:
  DuplicateLayerCmd(RegistryRef registry, layer_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::DuplicateLayer;
  }

 private:
  RegistryRef mRegistry;
  layer_id mLayerId;
  maybe<layer_id> mNewLayerId;
};

}  // namespace tactile