#pragma once

#include "map.hpp"
#include "map_command.hpp"
#include "position.hpp"

namespace tactile::cmd {

// TODO come up with better name
class remove_row_col : public map_command
{
 public:
  remove_row_col(core::map* map, const QString& name);

  auto mergeWith(const QUndoCommand* other) -> bool final;

 protected:
  [[nodiscard]] auto times() const noexcept -> int
  {
    return m_times;
  }

 private:
  int m_times{1};
};

}  // namespace tactile::cmd
