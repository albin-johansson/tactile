#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateGui(const Model& model, entt::dispatcher& dispatcher);

}  // namespace Tactile
