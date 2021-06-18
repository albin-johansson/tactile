#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "tileset_id.hpp"
#include "to_underlying.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

class SetTilesetName final : public QUndoCommand
{
 public:
  SetTilesetName(NotNull<core::MapDocument*> document,
                 tileset_id id,
                 QString name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::SetTilesetName);
  }

 private:
  core::MapDocument* mDocument{};
  tileset_id mId;
  QString mName;
  Maybe<QString> mPrevious;
};

}  // namespace tactile::cmd
