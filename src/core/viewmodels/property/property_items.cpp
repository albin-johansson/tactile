#include "property_items.hpp"

#include "tactile_error.hpp"

namespace tactile::vm {

void UpdateItemData(NotNull<QStandardItem*> item,
                    const core::Property& property)
{
  Q_ASSERT(item);
  switch (static_cast<vm::PropertyItemType>(item->type()))
  {
    case PropertyItemType::String:
    {
      item->setData(property.AsString(), Qt::EditRole);
      break;
    }
    case PropertyItemType::Integer:
    {
      item->setData(property.AsInteger(), Qt::EditRole);
      break;
    }
    case PropertyItemType::Floating:
    {
      item->setData(property.AsFloating(), Qt::EditRole);
      break;
    }
    case PropertyItemType::Boolean:
    {
      item->setData(property.AsBoolean() ? Qt::Checked : Qt::Unchecked,
                    Qt::CheckStateRole);
      break;
    }
    case PropertyItemType::File:
    {
      item->setData(property.AsFile().absoluteFilePath(),
                    vm::PropertyItemRole::Path);
      break;
    }
    case PropertyItemType::Color:
    {
      item->setData(property.AsColor(), vm::PropertyItemRole::Color);
      break;
    }
    case PropertyItemType::Object:
    {
      item->setData(property.AsObject().get(), Qt::EditRole);
      break;
    }
    default:
      throw TactileError{"Not a property item!"};
  }
}

auto ItemToProperty(NotNull<const QStandardItem*> item) -> core::Property
{
  Q_ASSERT(item);
  switch (static_cast<vm::PropertyItemType>(item->type()))
  {
    case PropertyItemType::String:
      return item->data(Qt::EditRole).value<QString>();

    case PropertyItemType::Integer:
      return item->data(Qt::EditRole).value<int>();

    case PropertyItemType::Floating:
      return item->data(Qt::EditRole).value<double>();

    case PropertyItemType::Boolean:
      return item->data(Qt::CheckStateRole).value<Qt::CheckState>() ==
             Qt::Checked;

    case PropertyItemType::File:
      return QFileInfo{item->data(vm::PropertyItemRole::Path).value<QString>()};

    case PropertyItemType::Color:
      return item->data(vm::PropertyItemRole::Color).value<QColor>();

    case PropertyItemType::Object:
      return core::object_ref{item->data(Qt::EditRole).value<int>()};

    default:
      throw TactileError{"Not a property item!"};
  }
}

}  // namespace tactile::vm
