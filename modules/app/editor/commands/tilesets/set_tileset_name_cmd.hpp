#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetTilesetNameCmd final : public command_base {
 public:
  SetTilesetNameCmd(registry_ref registry, tileset_id id, std::string name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::SetTilesetName;
  }

 private:
  registry_ref mRegistry;
  tileset_id mTilesetId;
  std::string mNewName;
  maybe<std::string> mOldName;
};

}  // namespace tactile
