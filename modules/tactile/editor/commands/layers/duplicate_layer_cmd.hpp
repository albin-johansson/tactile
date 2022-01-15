#pragma once

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class DuplicateLayerCmd final : public ACommand {
 public:
  DuplicateLayerCmd(RegistryRef registry, LayerID id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::DuplicateLayer;
  }

 private:
  RegistryRef mRegistry;
  LayerID mLayerId;
  Maybe<LayerID> mNewLayerId;
};

}  // namespace Tactile