#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetLayerVisibilityCmd final : public ACommand {
 public:
  SetLayerVisibilityCmd(RegistryRef registry, LayerID id, bool visible);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetLayerVisible;
  }

 private:
  RegistryRef mRegistry;
  LayerID mLayerId;
  bool mVisible;
  Maybe<bool> mPreviousVisibility;
};

}  // namespace tactile