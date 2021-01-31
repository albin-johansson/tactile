#include "make_property_item.hpp"

#include "icons.hpp"
#include "property_items.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

auto make_property_item(const core::property& property) -> QStandardItem*
{
  switch (property.get_type().value()) {
    case core::property::string: {
      auto* item = new string_item{};
      item->setData(property.as_string(), Qt::EditRole);
      return item;
    }
    case core::property::integer: {
      auto* item = new int_item{};
      item->setData(property.as_integer(), Qt::EditRole);
      return item;
    }
    case core::property::floating: {
      auto* item = new float_item{};
      item->setData(property.as_floating(), Qt::EditRole);
      return item;
    }
    case core::property::boolean: {
      auto* item = new bool_item{};
      item->setData(property.as_boolean() ? Qt::Checked : Qt::Unchecked,
                    Qt::CheckStateRole);
      return item;
    }
    case core::property::file: {
      auto* item = new file_item{};
      item->setData(icons::copy(), Qt::DecorationRole);
      item->setData(property.as_file().filePath(),
                    vm::property_item_role::path);
      return item;
    }
    case core::property::color: {
      auto* item = new color_item{};
      item->setData(property.as_color(), property_item_role::color);
      return item;
    }
    case core::property::object: {
      auto* item = new object_item{};
      item->setData(property.as_object().get(), Qt::EditRole);
      return item;
    }
    default:
      throw tactile_error{"Did not recognize property type!"};
  }
}

}  // namespace tactile::vm
