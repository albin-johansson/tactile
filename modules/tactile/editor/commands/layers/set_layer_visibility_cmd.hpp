#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class SetLayerVisibilityCmd final : public ACommand {
 public:
  SetLayerVisibilityCmd(Ref<entt::registry> registry, LayerID id, bool visible);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetLayerVisible;
  }

 private:
  Ref<entt::registry> mRegistry;
  LayerID mLayerId;
  bool mVisible;
  Maybe<bool> mPreviousVisibility;
};

}  // namespace Tactile