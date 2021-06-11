#include "make_property_item.hpp"

#include "icons.hpp"
#include "property_items.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

auto MakePropertyItem(const core::Property& property) -> QStandardItem*
{
  switch (property.Type().value())
  {
    case core::PropertyType::String:
    {
      auto* item = new string_item{};
      item->setData(property.AsString(), Qt::EditRole);
      return item;
    }
    case core::PropertyType::Integer:
    {
      auto* item = new int_item{};
      item->setData(property.AsInteger(), Qt::EditRole);
      return item;
    }
    case core::PropertyType::Floating:
    {
      auto* item = new float_item{};
      item->setData(property.AsFloating(), Qt::EditRole);
      return item;
    }
    case core::PropertyType::Boolean:
    {
      auto* item = new bool_item{};
      item->setData(property.AsBoolean() ? Qt::Checked : Qt::Unchecked,
                    Qt::CheckStateRole);
      return item;
    }
    case core::PropertyType::File:
    {
      auto* item = new file_item{};
      item->setData(IconCopy(), Qt::DecorationRole);
      item->setData(property.AsFile().filePath(), vm::PropertyItemRole::Path);
      return item;
    }
    case core::PropertyType::Color:
    {
      auto* item = new color_item{};
      item->setData(property.AsColor(), PropertyItemRole::Color);
      return item;
    }
    case core::PropertyType::Object:
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
