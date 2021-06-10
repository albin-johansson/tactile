#include "property_items.hpp"

#include "tactile_error.hpp"

namespace tactile::vm {

void update_item_data(not_null<QStandardItem*> item,
                      const core::Property& property)
{
  Q_ASSERT(item);
  switch (static_cast<vm::property_item_type>(item->type()))
  {
    case property_item_type::string:
    {
      item->setData(property.AsString(), Qt::EditRole);
      break;
    }
    case property_item_type::integer:
    {
      item->setData(property.AsInteger(), Qt::EditRole);
      break;
    }
    case property_item_type::floating:
    {
      item->setData(property.AsFloating(), Qt::EditRole);
      break;
    }
    case property_item_type::boolean:
    {
      item->setData(property.AsBoolean() ? Qt::Checked : Qt::Unchecked,
                    Qt::CheckStateRole);
      break;
    }
    case property_item_type::file:
    {
      item->setData(property.AsFile().absoluteFilePath(),
                    vm::property_item_role::path);
      break;
    }
    case property_item_type::color:
    {
      item->setData(property.AsColor(), vm::property_item_role::color);
      break;
    }
    case property_item_type::object:
    {
      item->setData(property.AsObject().get(), Qt::EditRole);
      break;
    }
    default:
      throw TactileError{"Not a property item!"};
  }
}

auto item_to_property(not_null<const QStandardItem*> item) -> core::Property
{
  Q_ASSERT(item);
  switch (static_cast<vm::property_item_type>(item->type()))
  {
    case property_item_type::string:
      return item->data(Qt::EditRole).value<QString>();

    case property_item_type::integer:
      return item->data(Qt::EditRole).value<int>();

    case property_item_type::floating:
      return item->data(Qt::EditRole).value<double>();

    case property_item_type::boolean:
      return item->data(Qt::CheckStateRole).value<Qt::CheckState>() ==
             Qt::Checked;

    case property_item_type::file:
      return QFileInfo{
          item->data(vm::property_item_role::path).value<QString>()};

    case property_item_type::color:
      return item->data(vm::property_item_role::color).value<QColor>();

    case property_item_type::object:
      return core::object_ref{item->data(Qt::EditRole).value<int>()};

    default:
      throw TactileError{"Not a property item!"};
  }
}

}  // namespace tactile::vm
