#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "map_document.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"

namespace tactile::cmd {

class RemoveTileset final : public QUndoCommand
{
 public:
  RemoveTileset(not_null<core::MapDocument*> document,
                Shared<core::Tileset> tileset,
                tileset_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::RemoveTileset);
  }

 private:
  core::MapDocument* mDocument{};
  Shared<core::Tileset> mTileset;
  tileset_id mId;
  bool mFirst{true};  ///< Used to determine when to notify UI
};

}  // namespace tactile::cmd
