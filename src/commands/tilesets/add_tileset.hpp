#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "map_document.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

class AddTileset final : public QUndoCommand
{
 public:
  AddTileset(not_null<core::MapDocument*> document,
             Shared<core::Tileset> tileset,
             tileset_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::AddTileset);
  }

 private:
  core::MapDocument* mDocument{};
  Shared<core::Tileset> mTileset;
  tileset_id mId;
};

}  // namespace tactile::cmd
