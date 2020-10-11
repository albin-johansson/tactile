#pragma once

#include "abstract_command.hpp"

namespace tactile::cmd {

class row_col_command : public abstract_command
{
 public:
  explicit row_col_command(const QString& name, core::map* map);

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
