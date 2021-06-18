#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "to_underlying.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

class SetLayerName final : public QUndoCommand
{
 public:
  SetLayerName(NotNull<core::MapDocument*> document,
               layer_id id,
               QString name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::SetLayerName);
  }

 private:
  NotNull<core::MapDocument*> mDocument{};
  layer_id mId;
  QString mName;
  Maybe<QString> mPrevious;
};

}  // namespace tactile::cmd
