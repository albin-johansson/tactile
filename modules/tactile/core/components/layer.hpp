#pragma once

#include <tactile_def.hpp>

namespace Tactile {

struct Layer final
{
  LayerID id;           ///< Unique layer ID.
  LayerType type{};     ///< The specific layer type.
  float opacity{1.0f};  ///< Transparency, in the range [0, 1].
  bool visible{true};   ///< Whether or not the layer is rendered.
};

struct ActiveLayer final
{
  entt::entity entity{entt::null};
};

}  // namespace Tactile
