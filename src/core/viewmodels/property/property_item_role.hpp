#pragma once

#include <Qt>

namespace tactile::vm {

struct PropertyItemRole final
{
  enum Value
  {
    Color = Qt::ItemDataRole::UserRole,
    Path
  };
};

}  // namespace tactile::vm
