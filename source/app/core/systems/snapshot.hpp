/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>  // string
#include <vector>  // vector

#include <boost/uuid/uuid_hash.hpp>

#include "core/attribute.hpp"
#include "core/common/associative.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"
#include "core/common/uuid.hpp"
#include "core/components/layers.hpp"
#include "core/components/objects.hpp"
#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"

namespace tactile::sys {

struct ComponentSnapshot final
{
  TreeMap<std::string, Attribute> attributes;
};

struct AttributeContextSnapshot final
{
  ContextID id{};
  std::string name;
  TreeMap<std::string, Attribute> properties;
  HashMap<ComponentID, ComponentSnapshot> components;
};

struct ObjectSnapshot final
{
  comp::Object core;
  AttributeContextSnapshot context;
};

struct LayerSnapshot final
{
  usize index{};
  comp::Layer core;
  AttributeContextSnapshot context;
  Maybe<UUID> parent;
  Maybe<TileMatrix> tiles;
  Maybe<std::vector<ObjectSnapshot>> objects;
  Maybe<std::vector<LayerSnapshot>> children;
};

}  // namespace tactile::sys