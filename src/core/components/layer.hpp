#pragma once

#include "aliases/ints.hpp"
#include "aliases/layer_id.hpp"
#include "core/layer_type.hpp"

namespace Tactile {

struct Layer final
{
  usize index{};        ///< Layer stack index relative to layer siblings.
  layer_id id;          ///< Unique layer ID.
  LayerType type{};     ///< The specific layer type.
  float opacity{1.0f};  ///< Transparency, in the range [0, 1].
  bool visible{true};   ///< Whether or not the layer is rendered.
};

struct ActiveLayer final
{
  entt::entity entity{entt::null};
};

}  // namespace Tactile
