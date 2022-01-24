#pragma once

#include <entt/entt.hpp>  // registry

namespace tactile {

class map_position;
struct RenderInfo;

void RenderStampPreview(const entt::registry& registry,
                        const map_position& mousePos,
                        const RenderInfo& info);

}  // namespace tactile
