// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/ir.hpp"

#include <algorithm>  // find_if

namespace tactile::ir {

auto get_attribute_type(const Component& component, StringView attr_name)
    -> Maybe<AttributeType>
{
  const auto attribute_iter =
      std::find_if(component.attributes.begin(),
                   component.attributes.end(),
                   [attr_name](const ir::NamedAttribute& stored_attribute) {
                     return stored_attribute.name == attr_name;
                   });

  if (attribute_iter != component.attributes.end()) {
    return attribute_iter->value.get_type();
  }

  return kNothing;
}

}  // namespace tactile::ir
