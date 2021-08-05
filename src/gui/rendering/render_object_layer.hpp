#pragma once

namespace Tactile {

class ObjectLayer;
struct RenderInfo;

void RenderObjectLayer(const ObjectLayer& layer,
                       const RenderInfo& info,
                       float parentOpacity);

}  // namespace Tactile
