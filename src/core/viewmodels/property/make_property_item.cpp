#include "make_property_item.hpp"

#include "icons.hpp"
#include "property_items.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

auto make_property_item(const core::property& property) -> QStandardItem*
{
  switch (property.type().value())
  {
    case core::property_type::string:
    {
      auto* item = new string_item{};
      item->setData(property.as_string(), Qt::EditRole);
      return item;
    }
    case core::property_type::integer:
    {
      auto* item = new int_item{};
      item->setData(property.as_integer(), Qt::EditRole);
      return item;
    }
    case core::property_type::floating:
    {
      auto* item = new float_item{};
      item->setData(property.as_floating(), Qt::EditRole);
      return item;
    }
    case core::property_type::boolean:
    {
      auto* item = new bool_item{};
      item->setData(property.as_boolean() ? Qt::Checked : Qt::Unchecked,
                    Qt::CheckStateRole);
      return item;
    }
    case core::property_type::file:
    {
      auto* item = new file_item{};
      item->setData(IconCopy(), Qt::DecorationRole);
      item->setData(property.as_file().filePath(),
                    vm::property_item_role::path);
      return item;
    }
    case core::property_type::color:
    {
      auto* item = new color_item{};
      item->setData(property.as_color(), property_item_role::color);
      return item;
    }
    case core::property_type::object:
    {
      auto* item = new object_item{};
      item->setData(property.as_object().get(), Qt::EditRole);
      return item;
    }
    default:
      throw tactile_error{"Did not recognize property type!"};
  }
}

}  // namespace tactile::vm
