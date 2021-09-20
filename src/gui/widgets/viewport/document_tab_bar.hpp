#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateDocumentTabBar(const Model& model, entt::dispatcher& dispatcher);

void CenterMapViewport();

}  // namespace Tactile
