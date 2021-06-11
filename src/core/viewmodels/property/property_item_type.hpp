#pragma once

#include <QStandardItem>

namespace tactile::vm {

enum class PropertyItemType
{
  String = QStandardItem::UserType,
  Integer,
  Floating,
  Boolean,
  File,
  Object,
  Color
};

}  // namespace tactile::vm
