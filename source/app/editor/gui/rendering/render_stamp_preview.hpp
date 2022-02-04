#pragma once

#include <entt/entt.hpp>  // registry

namespace tactile {

class tile_position;
struct RenderInfo;

void RenderStampPreview(const entt::registry& registry,
                        const tile_position& mousePos,
                        const RenderInfo& info);

}  // namespace tactile
