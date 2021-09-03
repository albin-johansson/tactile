#pragma once

#include <entt.hpp>  // registry, entity

#include "core/map_position.hpp"

struct ImVec2;

namespace Tactile {

void RenderStampPreview(const entt::registry& registry,
                        entt::entity tilesetEntity,
                        const ImVec2& mapPos,
                        const ImVec2& gridSize,
                        const MapPosition& mousePos);

}  // namespace Tactile
