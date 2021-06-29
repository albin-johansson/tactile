#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void MapContentViewport(const Model& model, entt::dispatcher& dispatcher);

void CenterMapContentViewport();

}  // namespace Tactile
