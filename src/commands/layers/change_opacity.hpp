#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "fwd.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

class ChangeOpacity final : public QUndoCommand
{
 public:
  ChangeOpacity(NotNull<core::MapDocument*> document,
                layer_id id,
                double opacity);

  void undo() override;

  void redo() override;

  auto mergeWith(const QUndoCommand* other) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::ChangeOpacity);
  }

 private:
  core::MapDocument* mDocument{};
  layer_id mId;
  double mOpacity;
  Maybe<double> mPreviousOpacity;
};

}  // namespace tactile::cmd
