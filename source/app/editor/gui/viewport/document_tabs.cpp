#include "document_tabs.hpp"

#include <imgui.h>

#include "core/components/attribute_context.hpp"
#include "editor/events/map_events.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "map_view.hpp"

namespace tactile {

void UpdateDocumentTabs(const document_model& model, entt::dispatcher& dispatcher)
{
  if (scoped::tab_bar bar{"##MapViewportTabBar", ImGuiTabBarFlags_Reorderable};
      bar.is_open()) {
    for (const auto& [id, document] : model) {
      const scoped::id scope{id};

      ImGuiTabItemFlags flags = 0;
      const auto isActive = model.active_map_id() == id;

      if (isActive) {
        flags |= ImGuiTabItemFlags_SetSelected;

        if (!model.is_clean()) {
          flags |= ImGuiTabItemFlags_UnsavedDocument;
        }
      }

      const auto& context = document->registry.ctx<comp::attribute_context>();
      bool opened = true;
      if (scoped::tab_item item{context.name.c_str(), &opened, flags}; item.is_open()) {
        if (isActive) {
          UpdateMapView(document->registry, dispatcher);
        }
      }

      if (!opened) {
        dispatcher.enqueue<close_map_event>(id);
      }
      else if (ImGui::IsItemActivated()) {
        dispatcher.enqueue<select_map_event>(id);
      }
    }
  }
}

}  // namespace tactile
