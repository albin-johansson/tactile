#include "make_property_item.hpp"

#include "icons.hpp"
#include "property_items.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

auto MakePropertyItem(const core::Property& property) -> QStandardItem*
{
  switch (property.Type().value())
  {
    case core::PropertyType::string:
    {
      auto* item = new string_item{};
      item->setData(property.AsString(), Qt::EditRole);
      return item;
    }
    case core::PropertyType::integer:
    {
      auto* item = new int_item{};
      item->setData(property.AsInteger(), Qt::EditRole);
      return item;
    }
    case core::PropertyType::floating:
    {
      auto* item = new float_item{};
      item->setData(property.AsFloating(), Qt::EditRole);
      return item;
    }
    case core::PropertyType::boolean:
    {
      auto* item = new bool_item{};
      item->setData(property.AsBoolean() ? Qt::Checked : Qt::Unchecked,
                    Qt::CheckStateRole);
      return item;
    }
    case core::PropertyType::file:
    {
      auto* item = new file_item{};
      item->setData(IconCopy(), Qt::DecorationRole);
      item->setData(property.AsFile().filePath(), vm::PropertyItemRole::Path);
      return item;
    }
    case core::PropertyType::color:
    {
      auto* item = new color_item{};
      item->setData(property.AsColor(), PropertyItemRole::Color);
      return item;
    }
    case core::PropertyType::object:
    {
      auto* item = new object_item{};
      item->setData(property.AsObject().get(), Qt::EditRole);
      return item;
    }
    default:
      throw TactileError{"Did not recognize property type!"};
  }
}

}  // namespace tactile::vm
