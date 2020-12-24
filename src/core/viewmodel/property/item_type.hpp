#pragma once

#include <QStandardItem>

namespace tactile::vm {

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

}  // namespace tactile::vm
