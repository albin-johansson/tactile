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

#include "core/common/memory.hpp"
#include "core/context.hpp"
#include "core/fwd.hpp"
#include "core/layer_type.hpp"

namespace tactile::core {

/// Interface for all layer classes.
class ILayer : public IContext
{
 public:
  virtual ~ILayer() noexcept override = default;

  virtual void accept(ILayerVisitor& visitor) = 0;
  virtual void accept(IConstLayerVisitor& visitor) const = 0;

  /// Sets the opacity of the layer, clamped to be within [0, 1].
  virtual void set_opacity(float opacity) = 0;

  /// Sets whether the layer is rendered.
  virtual void set_visible(bool visible) = 0;

  /// Returns the layer opacity, in the range [0, 1].
  [[nodiscard]] virtual auto get_opacity() const -> float = 0;

  /// Indicates whether the layer is rendered.
  [[nodiscard]] virtual auto is_visible() const -> bool = 0;

  /// Clones the layer (with a different UUID).
  [[nodiscard]] virtual auto clone() const -> Shared<ILayer> = 0;

  /// Returns the type tag of the layer instance.
  [[nodiscard]] virtual auto get_type() const -> LayerType = 0;
};

}  // namespace tactile::core
