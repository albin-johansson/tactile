#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateToolbarWidget(const Model& model, entt::dispatcher& dispatcher);

}  // namespace Tactile
