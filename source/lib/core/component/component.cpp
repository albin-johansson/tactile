// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "component.hpp"

#include <utility>  // move

namespace tactile {

Component::Component(const UUID& definition_id, AttributeMap attributes)
    : ComponentBase {definition_id}
{
  mAttributes = std::move(attributes);
}

auto Component::definition_id() const -> const UUID&
{
  return mTypeId;
}

}  // namespace tactile
