#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "map.hpp"

namespace tactile::cmd {

class repeated_command : public QUndoCommand
{
 public:
  explicit repeated_command(const QString& name) : QUndoCommand{name}
  {}

  auto mergeWith(const QUndoCommand* other) -> bool final;

 protected:
  [[nodiscard]] auto amount() const noexcept -> int
  {
    return m_amount;
  }

 private:
  int m_amount{1};
};

}  // namespace tactile::cmd
