#include "render_object_layer.hpp"

#include <algorithm>  // min

#include <imgui.h>
#include <imgui_internal.h>

#include "assert.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/attribute_context.hpp"
#include "graphics.hpp"

namespace tactile {
namespace {

void RenderPointObject(Graphics& graphics,
                       const entt::registry& registry,
                       const entt::entity objectEntity,
                       const ImVec2& position,
                       const cen::color& color)
{
  const float radius = std::min(graphics.GetViewportTileSize().x / 4.0f, 6.0f);

  const auto& object = registry.get<Object>(objectEntity);
  TACTILE_ASSERT(object.type == ObjectType::Point);

  if (graphics.IsWithinTranslatedBounds(position)) {
    graphics.SetDrawColor(color);
    graphics.SetLineThickness(2.0f);
    graphics.DrawTranslatedCircleWithShadow(position, radius);

    const auto& context = registry.get<attribute_context>(objectEntity);
    if (!context.name.empty()) {
      const auto* name = context.name.c_str();
      const auto textSize = ImGui::CalcTextSize(name);
      if (textSize.x <= graphics.GetViewportTileSize().x) {
        const auto textX = position.x - (textSize.x / 2.0f);
        const auto textY = position.y + radius + 4.0f;

        graphics.SetDrawColor(cen::colors::white);
        graphics.RenderTranslatedText(name, {textX, textY});
      }
    }
  }
}

void RenderEllipseObject(Graphics& graphics,
                         const entt::registry& registry,
                         const entt::entity objectEntity,
                         const ImVec2& position,
                         const cen::color& color)
{
  const auto& object = registry.get<Object>(objectEntity);
  const auto& context = registry.get<attribute_context>(objectEntity);
  TACTILE_ASSERT(object.type == ObjectType::Ellipse);

  const ImVec2 size = {object.width, object.height};

  const auto radius = ImVec2{0.5f, 0.5f} * size * graphics.GetTileSizeRatio();
  const auto center = position + radius;

  graphics.SetDrawColor(color);
  graphics.SetLineThickness(2);
  graphics.DrawTranslatedEllipseWithShadow(center, radius);

  if (!context.name.empty()) {
    const CStr text = context.name.c_str();
    const auto textSize = ImGui::CalcTextSize(text);
    if (textSize.x <= radius.x) {
      const auto textX = center.x - (textSize.x / 2.0f);
      const auto textY = center.y + (textSize.y / 2.0f) + (radius.y);

      graphics.SetDrawColor(cen::colors::white);
      graphics.RenderTranslatedText(text, {textX, textY});
    }
  }
}

void RenderRectangleObject(Graphics& graphics,
                           const entt::registry& registry,
                           const entt::entity objectEntity,
                           const ImVec2& position,
                           const cen::color& color)
{
  const auto& object = registry.get<Object>(objectEntity);
  TACTILE_ASSERT(object.type == ObjectType::Rectangle);

  const auto size = ImVec2{object.width, object.height} * graphics.GetTileSizeRatio();

  if (graphics.IsIntersectingBounds(position, size)) {
    graphics.SetDrawColor(color);
    graphics.SetLineThickness(2.0f);
    graphics.DrawTranslatedRectWithShadow(position, size);

    const auto& context = registry.get<attribute_context>(objectEntity);
    if (!context.name.empty()) {
      const auto* name = context.name.c_str();
      const auto textSize = ImGui::CalcTextSize(name);
      if (textSize.x <= size.x) {
        const auto textX = (size.x - textSize.x) / 2.0f;

        graphics.SetDrawColor(cen::colors::white.with_alpha(color.alpha()));
        graphics.RenderTranslatedText(name, position + ImVec2{textX, size.y + 4.0f});
      }
    }
  }
}

}  // namespace

void RenderObject(Graphics& graphics,
                  const entt::registry& registry,
                  const entt::entity objectEntity,
                  const cen::color& color)
{
  const auto& object = registry.get<Object>(objectEntity);

  if (!object.visible) {
    return;
  }

  const auto position = ImVec2{object.x, object.y} * graphics.GetTileSizeRatio();

  switch (object.type) {
    case ObjectType::Point:
      RenderPointObject(graphics, registry, objectEntity, position, color);
      break;

    case ObjectType::Ellipse:
      RenderEllipseObject(graphics, registry, objectEntity, position, color);
      break;

    case ObjectType::Rectangle:
      RenderRectangleObject(graphics, registry, objectEntity, position, color);
      break;
  }
}

void RenderObjectLayer(Graphics& graphics,
                       const entt::registry& registry,
                       const entt::entity layerEntity,
                       const float parentOpacity)
{
  const auto& layer = registry.get<Layer>(layerEntity);
  const auto& objectLayer = registry.get<ObjectLayer>(layerEntity);

  const auto opacity = parentOpacity * layer.opacity;
  const auto objectColor = cen::color::from_norm(1, 0, 0, opacity);

  for (const auto objectEntity : objectLayer.objects) {
    RenderObject(graphics, registry, objectEntity, objectColor);
  }
}

}  // namespace tactile
