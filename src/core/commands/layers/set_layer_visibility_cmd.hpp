#pragma once

#include <entt.hpp>  // registry

#include "common/layer_id.hpp"
#include "common/maybe.hpp"
#include "common/ref.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"

namespace Tactile {

class SetLayerVisibilityCmd final : public ACommand
{
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