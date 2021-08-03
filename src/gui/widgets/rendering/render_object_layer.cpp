#include "render_object_layer.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/map/layers/object_layer.hpp"
#include "gui/widgets/rendering/render_bounds.hpp"
#include "gui/widgets/rendering/render_info.hpp"

namespace Tactile {
namespace {

void RenderPoint(const Object& object,
                 const ImVec2& position,
                 const cen::frect& boundsRect,
                 const ImU32 color,
                 const float gridWidth)
{
  constexpr float radius = 6.0f;

  if (boundsRect.contains(cen::fpoint{position.x, position.y}))
  {
    auto* drawList = ImGui::GetWindowDrawList();

    drawList->AddCircle(position + ImVec2{2, 2}, radius, IM_COL32_BLACK, 0, 2.0f);
    drawList->AddCircle(position, radius, color, 0, 2.0f);

    if (const auto name = object.GetName(); !name.empty())
    {
      const auto textSize = ImGui::CalcTextSize(name.data());
      if (textSize.x <= gridWidth)
      {
        const auto textX = position.x - (textSize.x / 2.0f);
        const auto textY = position.y + radius + 4.0f;
        drawList->AddText({textX, textY}, IM_COL32_WHITE, name.data());
      }
    }
  }
}

void RenderRect(const Object& object,
                const ImVec2& position,
                const cen::frect& boundsRect,
                const ImU32 color,
                const ImVec2& ratio)
{
  const auto size = ImVec2{object.GetWidth(), object.GetHeight()} * ratio;
  const auto rect = cen::frect{position.x, position.y, size.x, size.y};
  if (cen::intersects(boundsRect, rect))
  {
    auto* drawList = ImGui::GetWindowDrawList();

    drawList->AddRect(position + ImVec2{2, 2},
                      position + ImVec2{2, 2} + size,
                      IM_COL32_BLACK,
                      0,
                      0,
                      2.0f);
    drawList->AddRect(position, position + size, color, 0, 0, 2.0f);

    if (const auto name = object.GetName(); !name.empty())
    {
      const auto textSize = ImGui::CalcTextSize(name.data());
      if (textSize.x <= size.x)
      {
        const auto textX = (size.x - textSize.x) / 2.0f;
        drawList->AddText(position + ImVec2{textX, size.y + 4.0f},
                          IM_COL32_WHITE,
                          name.data());
      }
    }
  }
}

}  // namespace

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
    else
    {
      RenderRect(object, absolutePos, rect, color, ratio);
    }
  }
}

}  // namespace Tactile
