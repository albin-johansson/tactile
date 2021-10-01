#pragma once

#include <entt.hpp>  // registry

#include "common/maybe.hpp"
#include "common/ref.hpp"
#include "common/tileset_id.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/systems/tileset_snapshot.hpp"

namespace Tactile {

class RemoveTilesetCmd final : public ACommand
{
 public:
  RemoveTilesetCmd(Ref<entt::registry> registry, TilesetID id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveTileset;
  }

 private:
  Ref<entt::registry> mRegistry;
  TilesetID mTilesetId;
  Maybe<Sys::TilesetSnapshot> mSnapshot;
};

}  // namespace Tactile
