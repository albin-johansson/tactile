#pragma once

#include <entt/entt.hpp>  // registry

namespace tactile {

class tile_position;
struct render_info;

void RenderStampPreview(const entt::registry& registry,
                        const tile_position& mousePos,
                        const render_info& info);

}  // namespace tactile
