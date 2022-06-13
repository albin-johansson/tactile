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

#include <boost/uuid/uuid_hash.hpp>

#include "core/common/associative.hpp"
#include "core/common/uuid.hpp"
#include "core/layers/layer.hpp"
#include "core/layers/layer_delegate.hpp"
#include "core/layers/object.hpp"

namespace tactile::core {

class ObjectLayer final : public ILayer
{
 public:
  [[nodiscard]] static auto make() -> Shared<ObjectLayer>;

  void accept(ILayerVisitor& visitor) override;
  void accept(IConstLayerVisitor& visitor) const override;

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  [[nodiscard]] auto get_opacity() const -> float override;

  [[nodiscard]] auto is_visible() const -> bool override;

  [[nodiscard]] auto clone() const -> Shared<ILayer> override;

  [[nodiscard]] auto get_props() -> PropertyBundle& override;
  [[nodiscard]] auto get_props() const -> const PropertyBundle& override;

  [[nodiscard]] auto get_comps() -> ComponentBundle& override;
  [[nodiscard]] auto get_comps() const -> const ComponentBundle& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto get_type() const -> LayerType override
  {
    return LayerType::ObjectLayer;
  }

 private:
  LayerDelegate mDelegate;
  HashMap<UUID, Object> mObjects;
};

}  // namespace tactile::core
