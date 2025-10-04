// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "component_builder.hpp"

#include <utility>  // move

namespace tactile::test {

ComponentBuilder::ComponentBuilder(Shared<ComponentIndex> index, std::string name)
    : mIndex {std::move(index)}
    , mComponentId {mIndex->define_comp(std::move(name))}
{}

auto ComponentBuilder::with_attr(std::string name, Attribute value) -> ComponentBuilder&
{
  auto& comp = mIndex->at(mComponentId);
  comp.add_attr(std::move(name), std::move(value));
  return *this;
}

}  // namespace tactile::test
