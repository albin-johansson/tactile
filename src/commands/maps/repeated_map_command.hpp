#pragma once

#include "forward_declare.hpp"
#include "map_command.hpp"
#include "not_null.hpp"
#include "position.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class RepeatedMapCommand : public MapCommand
{
 public:
  RepeatedMapCommand(not_null<core::map_document*> document,
                     const QString& name);

  auto mergeWith(const QUndoCommand* other) -> bool final;

 protected:
  [[nodiscard]] auto Times() const noexcept -> int
  {
    return mTimes;
  }

 private:
  int mTimes{1};
};

}  // namespace tactile::cmd
