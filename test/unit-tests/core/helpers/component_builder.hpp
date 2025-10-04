// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <string>  // string

#include "core/attribute.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/comp/component_index.hpp"

namespace tactile::test {

class ComponentBuilder
{
 public:
  ComponentBuilder(Shared<ComponentIndex> index, std::string name);

  auto with_attr(std::string name, Attribute value = std::string {}) -> ComponentBuilder&;

  [[nodiscard]] auto result() -> UUID { return mComponentId; }

 private:
  Shared<ComponentIndex> mIndex;
  UUID                   mComponentId;
};

}  // namespace tactile::test
