#pragma once

#include <QUndoCommand>  // QUndoCommand

namespace tactile::cmd {

class RepeatedCommand : public QUndoCommand
{
 public:
  explicit RepeatedCommand(const QString& name) : QUndoCommand{name}
  {}

  auto mergeWith(const QUndoCommand* other) -> bool final;

 protected:
  [[nodiscard]] auto Amount() const noexcept -> int
  {
    return mAmount;
  }

 private:
  int mAmount{1};
};

}  // namespace tactile::cmd
