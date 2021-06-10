#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class SetLayerName final : public QUndoCommand
{
 public:
  SetLayerName(not_null<core::map_document*> document,
               layer_id id,
               QString name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::set_layer_name);
  }

 private:
  not_null<core::map_document*> mDocument{};
  layer_id mId;
  QString mName;
  maybe<QString> mPrevious;
};

}  // namespace tactile::cmd
