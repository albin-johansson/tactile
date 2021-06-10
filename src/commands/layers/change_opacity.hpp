#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class ChangeOpacity final : public QUndoCommand
{
 public:
  ChangeOpacity(not_null<core::map_document*> document,
                layer_id id,
                double opacity);

  void undo() override;

  void redo() override;

  auto mergeWith(const QUndoCommand* other) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::change_opacity);
  }

 private:
  core::map_document* mDocument{};
  layer_id mId;
  double mOpacity;
  maybe<double> mPreviousOpacity;
};

}  // namespace tactile::cmd
