#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Model;

void UpdateToolbarWidget(const Model& model, entt::dispatcher& dispatcher);

}  // namespace tactile
