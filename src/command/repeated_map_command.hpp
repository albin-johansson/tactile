#pragma once

#include "map.hpp"
#include "map_command.hpp"
#include "position.hpp"

namespace tactile::cmd {

class repeated_map_command : public map_command
{
 public:
  repeated_map_command(core::map* map, const QString& name);

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
