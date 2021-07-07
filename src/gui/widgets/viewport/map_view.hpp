#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;
class MapDocument;

void MapView(const Model& model,
             const MapDocument& document,
             entt::dispatcher& dispatcher);

}  // namespace Tactile
