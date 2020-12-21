#pragma once

#include <QStandardItem>

namespace tactile::viewmodel {

enum class item_type
{
  name = QStandardItem::UserType,
  string,
  integer,
  floating,
  boolean,
  file,
  object,
  color
};

}  // namespace tactile::viewmodel
