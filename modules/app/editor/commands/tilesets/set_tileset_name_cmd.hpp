#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetTilesetNameCmd final : public ACommand {
 public:
  SetTilesetNameCmd(RegistryRef registry, TilesetID id, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetTilesetName;
  }

 private:
  RegistryRef mRegistry;
  TilesetID mTilesetId;
  std::string mNewName;
  Maybe<std::string> mOldName;
};

}  // namespace tactile
