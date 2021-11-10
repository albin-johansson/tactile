#pragma once

#include <entt/entt.hpp>  // registry

namespace Tactile {

class MapPosition;
struct RenderInfo;

void RenderStampPreview(const entt::registry& registry,
                        const MapPosition& mousePos,
                        const RenderInfo& info);

}  // namespace Tactile
