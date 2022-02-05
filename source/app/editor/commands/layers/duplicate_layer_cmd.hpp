#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class DuplicateLayerCmd final : public command_base {
 public:
  DuplicateLayerCmd(registry_ref registry, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::duplicate_layer;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayerId;
  maybe<layer_id> mNewLayerId;
};

}  // namespace tactile