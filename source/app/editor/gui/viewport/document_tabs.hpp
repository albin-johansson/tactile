#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace tactile {

class Model;

void UpdateDocumentTabs(const Model& model, entt::dispatcher& dispatcher);

void CenterMapViewport();

}  // namespace tactile
