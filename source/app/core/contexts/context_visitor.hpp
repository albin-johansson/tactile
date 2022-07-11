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

#include "core/fwd.hpp"

namespace tactile {

/// An interface for visiting specific context implementations.
class IContextVisitor
{
 public:
  virtual ~IContextVisitor() noexcept = default;

  virtual void visit([[maybe_unused]] const Map& map) {}
  virtual void visit([[maybe_unused]] const TileLayer& layer) {}
  virtual void visit([[maybe_unused]] const ObjectLayer& layer) {}
  virtual void visit([[maybe_unused]] const GroupLayer& layer) {}
  virtual void visit([[maybe_unused]] const Object& object) {}
  virtual void visit([[maybe_unused]] const Tileset& tileset) {}
  virtual void visit([[maybe_unused]] const Tile& tile) {}
};

}  // namespace tactile
