#pragma once

#include <string>  // string

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class set_tileset_name_cmd final : public command_base {
 public:
  set_tileset_name_cmd(registry_ref registry, tileset_id id, std::string name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::set_tileset_name;
  }

 private:
  registry_ref mRegistry;
  tileset_id mTilesetId;
  std::string mNewName;
  maybe<std::string> mOldName;
};

}  // namespace tactile
