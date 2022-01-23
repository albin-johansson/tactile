#pragma once

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/components/texture.hpp"
#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class AddTilesetCmd final : public ACommand {
 public:
  AddTilesetCmd(RegistryRef registry, Texture texture, int tileWidth, int tileHeight);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddTileset;
  }

 private:
  RegistryRef mRegistry;
  Texture mTexture;
  int mTileWidth;
  int mTileHeight;
  Maybe<TilesetID> mTilesetId;
  Maybe<sys::TilesetSnapshot> mSnapshot;
};

}  // namespace tactile
