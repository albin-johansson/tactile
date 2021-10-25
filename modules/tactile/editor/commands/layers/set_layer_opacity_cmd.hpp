#pragma once

#include <entt.hpp>  // registry

#include <tactile-base/tactile_std.hpp>

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class SetLayerOpacityCmd final : public ACommand
{
 public:
  SetLayerOpacityCmd(Ref<entt::registry> registry, LayerID id, float opacity);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetLayerOpacity;
  }

 private:
  Ref<entt::registry> mRegistry;
  LayerID mLayerId;
  float mOpacity;
  Maybe<float> mPreviousOpacity;
};

}  // namespace Tactile
