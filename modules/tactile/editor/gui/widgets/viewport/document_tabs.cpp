#include "document_tabs.hpp"

#include <imgui.h>

#include "map_view.hpp"
#include "tactile/core/components/property_context.hpp"
#include "tactile/core/utils/scope_id.hpp"
#include "tactile/editor/events/map_events.hpp"
#include "tactile/editor/model.hpp"

namespace Tactile {

void UpdateDocumentTabs(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginTabBar("MapViewportTabBar", ImGuiTabBarFlags_Reorderable)) {
    for (const auto& [id, document] : model) {
      const ScopeID uid{id};

      ImGuiTabItemFlags flags = 0;
      const auto isActive = model.GetActiveMapId() == id;

      if (isActive) {
        flags |= ImGuiTabItemFlags_SetSelected;

        if (!model.IsClean()) {
          flags |= ImGuiTabItemFlags_UnsavedDocument;
        }
      }

      const auto& context = document->registry.ctx<PropertyContext>();
      bool opened = true;
      if (ImGui::BeginTabItem(context.name.c_str(), &opened, flags)) {
        if (isActive) {
          UpdateMapView(document->registry, dispatcher);
        }

        ImGui::EndTabItem();
      }

      if (!opened) {
        dispatcher.enqueue<CloseMapEvent>(id);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<SelectMapEvent>(id);
      }
    }

    ImGui::EndTabBar();
  }
}

}  // namespace Tactile
