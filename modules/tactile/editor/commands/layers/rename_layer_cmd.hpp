#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class RenameLayerCmd final : public ACommand {
 public:
  RenameLayerCmd(Ref<entt::registry> registry, LayerID id, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RenameLayer;
  }

 private:
  Ref<entt::registry> mRegistry;
  LayerID mLayerId;
  std::string mName;
  Maybe<std::string> mPreviousName;
};

}  // namespace Tactile
