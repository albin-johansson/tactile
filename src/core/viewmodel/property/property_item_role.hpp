#pragma once

#include <Qt>

namespace tactile::viewmodel {

class property_item_role final
{
 public:
  enum role
  {
    color = Qt::ItemDataRole::UserRole,
    path
  };
};

}  // namespace tactile::viewmodel
