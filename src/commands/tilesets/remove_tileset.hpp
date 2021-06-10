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
  RemoveTileset(not_null<core::map_document*> document,
                shared<core::tileset> tileset,
                tileset_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::remove_tileset);
  }

 private:
  core::map_document* mDocument{};
  shared<core::tileset> mTileset;
  tileset_id mId;
  bool mFirst{true};  ///< Used to determine when to notify UI
};

}  // namespace tactile::cmd
