#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class RenameLayerCmd final : public command_base {
 public:
  RenameLayerCmd(registry_ref registry, layer_id id, std::string name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::rename_layer;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayerId;
  std::string mName;
  maybe<std::string> mPreviousName;
};

}  // namespace tactile
