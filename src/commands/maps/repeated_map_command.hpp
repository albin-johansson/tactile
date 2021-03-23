#pragma once

#include "forward_declare.hpp"
#include "map_command.hpp"
#include "not_null.hpp"
#include "position.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class repeated_map_command : public map_command
{
 public:
  repeated_map_command(not_null<core::map_document*> document,
                       const QString& name);

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
