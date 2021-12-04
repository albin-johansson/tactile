#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateDocumentTabs(const Model& model, entt::dispatcher& dispatcher);

void CenterMapViewport();

}  // namespace Tactile