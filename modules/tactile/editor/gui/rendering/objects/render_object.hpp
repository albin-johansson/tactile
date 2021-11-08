#pragma once

#include <tactile_def.hpp>

namespace Tactile {

struct RenderInfo;

void RenderObject(const entt::registry& registry,
                  entt::entity objectEntity,
                  const RenderInfo& info,
                  uint32 color);

}  // namespace Tactile