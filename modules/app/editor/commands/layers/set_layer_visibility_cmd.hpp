#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetLayerVisibilityCmd final : public ACommand {
 public:
  SetLayerVisibilityCmd(RegistryRef registry, layer_id id, bool visible);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetLayerVisible;
  }

 private:
  RegistryRef mRegistry;
  layer_id mLayerId;
  bool mVisible;
  maybe<bool> mPreviousVisibility;
};

}  // namespace tactile