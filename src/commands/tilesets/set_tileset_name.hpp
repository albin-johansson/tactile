#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "tileset_id.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class SetTilesetName final : public QUndoCommand
{
 public:
  SetTilesetName(not_null<core::map_document*> document,
                 tileset_id id,
                 QString name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::set_tileset_name);
  }

 private:
  core::map_document* mDocument{};
  tileset_id mId;
  QString mName;
  maybe<QString> mPrevious;
};

}  // namespace tactile::cmd
