#pragma once

#include <Qt>

namespace tactile::vm {

struct LayerItemRole final
{
  enum Value
  {
    Id = Qt::UserRole,
    Type
  };
};

}  // namespace tactile::vm
