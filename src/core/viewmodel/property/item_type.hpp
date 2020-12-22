#pragma once

#include <QStandardItem>

namespace tactile::viewmodel {

enum class item_type
{
  string = QStandardItem::UserType,
  integer,
  floating,
  boolean,
  file,
  object,
  color
};

}  // namespace tactile::viewmodel
