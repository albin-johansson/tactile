#include "property_items.hpp"

#include "tactile_error.hpp"

namespace tactile::vm {

void update_item_data(not_null<QStandardItem*> item, const core::property& p)
{
  Q_ASSERT(item);
  switch (static_cast<vm::property_item_type>(item->type())) {
    case property_item_type::string: {
      item->setData(p.as_string(), Qt::EditRole);
      break;
    }
    case property_item_type::integer: {
      item->setData(p.as_integer(), Qt::EditRole);
      break;
    }
    case property_item_type::floating: {
      item->setData(p.as_floating(), Qt::EditRole);
      break;
    }
    case property_item_type::boolean: {
      item->setData(p.as_boolean() ? Qt::Checked : Qt::Unchecked,
                    Qt::CheckStateRole);
      break;
    }
    case property_item_type::file: {
      item->setData(p.as_file().absoluteFilePath(),
                    vm::property_item_role::path);
      break;
    }
    case property_item_type::color: {
      item->setData(p.as_color(), vm::property_item_role::color);
      break;
    }
    case property_item_type::object: {
      [[fallthrough]];  // FIXME
    }
    default: {
      throw tactile_error{"Not a property item!"};
    }
  }
}

auto item_to_property(not_null<const QStandardItem*> item) -> core::property
{
  Q_ASSERT(item);
  switch (static_cast<vm::property_item_type>(item->type())) {
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
      [[fallthrough]];  // FIXME

    default:
      throw tactile_error{"Not a property item!"};
  }
}

}  // namespace tactile::vm
