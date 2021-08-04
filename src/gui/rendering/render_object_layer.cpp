#include "render_object_layer.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "common.hpp"
#include "core/map/layers/object_layer.hpp"
#include "objects/render_ellipse.hpp"
#include "objects/render_point.hpp"
#include "objects/render_rect.hpp"
#include "render_bounds.hpp"
#include "render_info.hpp"

namespace Tactile {

void RenderObjectLayer(const ObjectLayer& layer, const RenderInfo& info)
{
  const auto opacity = 255.0f * layer.GetOpacity();
  const auto color = IM_COL32(0xFF, 0, 0, opacity);

  const auto ratio = info.grid_size / info.tile_size;
  const auto rect = ConvertBoundsToRect(info);

  for (const auto& [id, object] : layer)
  {
    const auto localPos = ImVec2{object.GetX(), object.GetY()};
    const auto absolutePos = info.map_position + (localPos * ratio);

    if (object.IsPoint())
    {
      RenderPoint(object, absolutePos, rect, color, info.grid_size.x);
    }
    else if (object.IsEllipse())
    {
      RenderEllipse(object, absolutePos, ratio, color);
    }
    else if (object.IsRectangle())
    {
      RenderRect(object, absolutePos, rect, color, ratio);
    }
    else
    {
      cen::log::warn("Did not recognize object type when rendering objects!");
    }
  }
}

}  // namespace Tactile
