#include "layer_widget.hpp"

#include <IconsFontAwesome5.h>

#include <format>         // format
#include <limits>         // numeric_limits
#include <string>         // string
#include <unordered_map>  // unordered_map

#include "aliases/czstring.hpp"
#include "core/events/select_layer_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/button_ex.hpp"
#include "imgui.h"

namespace tactile {
namespace {

inline bool is_visible = true;
inline std::unordered_map<layer_id, std::string> names;

void UpdateLayers(const MapDocument& document, entt::dispatcher& dispatcher)
{
  const auto& map = document.GetMap();
  const auto activeLayerId = map.GetActiveLayerId();
  for (const auto& [id, layer] : map)
  {
    if (!names.contains(id))
    {
      const auto icon = layer->GetType() == LayerType::TileLayer
                            ? ICON_FA_LAYER_GROUP
                            : ICON_FA_SHAPES;

      names.try_emplace(
          id,
          std::format("{} {}##{}", icon, layer->GetName(), id.get()));
    }

    const auto& name = names.at(id);

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

}  // namespace tactile
