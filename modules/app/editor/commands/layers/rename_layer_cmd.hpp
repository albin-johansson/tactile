#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RenameLayerCmd final : public ACommand {
 public:
  RenameLayerCmd(RegistryRef registry, layer_id id, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RenameLayer;
  }

 private:
  RegistryRef mRegistry;
  layer_id mLayerId;
  std::string mName;
  maybe<std::string> mPreviousName;
};

}  // namespace tactile
