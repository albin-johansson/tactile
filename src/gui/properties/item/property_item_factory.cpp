#include "property_item_factory.hpp"

#include "property_bool_item.hpp"
#include "property_color_item.hpp"
#include "property_file_item.hpp"
#include "property_float_item.hpp"
#include "property_int_item.hpp"
#include "property_object_item.hpp"
#include "property_string_item.hpp"
#include "tactile_error.hpp"

namespace tactile::gui {

auto property_item_factory::make(const QString& name,
                                 const core::property& property,
                                 QTreeWidgetItem* parent) -> property_tree_item*
{
  Q_ASSERT(property.has_value());

  switch (*property.get_type()) {
    case core::property::string:
      return new property_string_item{name, property, parent};

    case core::property::integer:
      return new property_int_item{name, property, parent};

    case core::property::floating:
      return new property_float_item{name, property, parent};

    case core::property::boolean:
      return new property_bool_item{name, property, parent};

    case core::property::file:
      return new property_file_item{name, property, parent};

    case core::property::color:
      return new property_color_item{name, property, parent};

    case core::property::object:
      return new property_object_item{name, property, parent};

    default:
      throw tactile_error{"Unknown property type!"};
  }
}

}  // namespace tactile::gui
