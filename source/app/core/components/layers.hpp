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

#include <vector>  // vector

#include <entt/entity/entity.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/layer_type.hpp"

namespace tactile {

using TileRow = std::vector<TileID>;
using TileMatrix = std::vector<TileRow>;

namespace comp {

/**
 * \brief Component that provides common aspects of layers.
 *
 * \details This component is used by all different kinds of layers.
 *
 * \see TileLayer
 * \see ObjectLayer
 * \see GroupLayer
 *
 * \ingroup components
 */
struct Layer final
{
  [[deprecated]] LayerID id{};  ///< Unique layer ID.
  LayerType type{};             ///< The specific layer type.
  float opacity{1.0f};          ///< Opacity of the layer, in the range [0, 1].
  bool visible{true};           ///< Whether or not the layer is rendered.
};

/**
 * \brief Component that represents plain tile layers.
 *
 * \ingroup components
 */
struct TileLayer final
{
  TileMatrix matrix;  ///< The associated matrix of tile identifiers.
};

/**
 * \brief Component that represents object layers.
 *
 * \see Object
 *
 * \ingroup components
 */
struct ObjectLayer final
{
  std::vector<entt::entity> objects;  ///< The associated object entities.
};

/**
 * \brief Tag component used to denote group layers.
 *
 * \ingroup components
 */
struct GroupLayer final
{};

/**
 * \brief Component used to represent the layer hierarchy.
 *
 * \ingroup components
 */
struct LayerTreeNode final
{
  usize index{};                       ///< Local index, i.e. relative to tree siblings.
  std::vector<entt::entity> children;  ///< All associated child nodes.
};

/**
 * \brief Context component that keeps track of the active layer.
 *
 * \ingroup components
 */
struct ActiveLayer final
{
  entt::entity entity{entt::null};  ///< The currently active layer, can be null.
};

}  // namespace comp
}  // namespace tactile
