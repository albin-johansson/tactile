#include "layer_widget.hpp"

#include <IconsFontAwesome5.h>

#include <format>  // format
#include <limits>  // numeric_limits

#include "aliases/czstring.hpp"
#include "core/events/select_layer_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/button_ex.hpp"
#include "imgui.h"
#include "utils/scope_id.hpp"

namespace Tactile {
namespace {

inline bool is_visible = true;

void UpdateLayers(const MapDocument& document, entt::dispatcher& dispatcher)
{
  const auto& map = document.GetMap();
  const auto activeLayerId = map.GetActiveLayerId();
  for (const auto& [id, layer] : map)
  {
    const ScopeID uid{id};

    const auto icon = layer->GetType() == LayerType::ObjectLayer
                          ? ICON_FA_SHAPES
                          : ICON_FA_LAYER_GROUP;
    const auto name = std::format("{} {}", icon, layer->GetName());

    const auto isSelected = id == activeLayerId;
    ImGui::Selectable(name.c_str(), isSelected);

    if (ImGui::IsItemActivated())
    {
      dispatcher.enqueue<SelectLayerEvent>(id);
    }

    if (isSelected)
    {
      ImGui::SetItemDefaultFocus();
    }
  }
}

}  // namespace

void UpdateLayerWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (!is_visible || !model.GetActiveMapId())
  {
    return;
  }

  if (ImGui::Begin("Layers", &is_visible, ImGuiWindowFlags_NoCollapse))
  {
    if (ButtonEx(ICON_FA_PLUS_CIRCLE, "Add new layer."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_MINUS_CIRCLE, "Remove layer."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_COPY, "Duplicate layer."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_ARROW_UP, "Move layer up."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_ARROW_DOWN, "Move layer down."))
    {}

    const auto windowHeight = ImGui::GetWindowHeight();
    const auto textLineHeight = ImGui::GetTextLineHeightWithSpacing();
    const auto size = ImVec2{std::numeric_limits<float>::min(),
                             windowHeight - (4 * textLineHeight)};

    if (ImGui::BeginListBox("##LayerTreeNode", size))
    {
      if (const auto* document = model.GetActiveDocument())
      {
        UpdateLayers(*document, dispatcher);
      }

      ImGui::EndListBox();
    }
  }

  ImGui::End();
}

void SetLayerWidgetVisible(const bool visible) noexcept
{
  is_visible = visible;
}

auto IsLayerWidgetVisible() noexcept -> bool
{
  return is_visible;
}

}  // namespace Tactile
