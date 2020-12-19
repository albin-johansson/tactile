#pragma once

#include <QStandardItem>

namespace tactile::gui {

enum class property_item_type
{
  name = QStandardItem::UserType,
  integer,
  floating,
  boolean,
  file,
  object,
  color
};

}
