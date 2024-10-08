// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/menu/map_menu.hpp"

#include <imgui.h>

#include "tactile/core/document/document_info.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/map_events.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/menus.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {

void MapMenu::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();

  const auto* map_document = dynamic_cast<const MapDocument*>(model.get_current_document());
  const DisabledScope disable_menu_if {map_document == nullptr};

  if (const MenuScope menu {language.get(StringID::kMapMenu)}; menu.is_open()) {
    const auto* registry = map_document ? &map_document->get_registry() : nullptr;
    const auto* document_info = registry ? registry->find<CDocumentInfo>() : nullptr;
    const auto* map = registry ? registry->find<CMap>(document_info->root) : nullptr;

    if (ImGui::MenuItem(language.get(StringID::kShowMetadata))) {
      dispatcher.push<InspectMapEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kAddTileset))) {
      dispatcher.push<ShowAddTilesetDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kResize))) {
      dispatcher.push<ShowResizeMapDialogEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kAddRow))) {
      dispatcher.push<AddRowToMapEvent>();
    }

    if (ImGui::MenuItem(language.get(StringID::kAddColumn))) {
      dispatcher.push<AddColumnToMapEvent>();
    }

    if (const DisabledScope disable_if {!map || map->extent.rows <= 1};
        ImGui::MenuItem(language.get(StringID::kRemoveRow))) {
      dispatcher.push<RemoveRowToMapEvent>();
    }

    if (const DisabledScope disable_if {!map || map->extent.cols <= 1};
        ImGui::MenuItem(language.get(StringID::kRemoveColumn))) {
      dispatcher.push<RemoveColumnToMapEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kFixInvalidTiles))) {
      dispatcher.push<FixMapTilesEvent>();
    }

    ImGui::Separator();

    _push_export_as_menu(language, dispatcher);
  }
}

void MapMenu::_push_export_as_menu(const Language& language, EventDispatcher& dispatcher)
{
  if (const MenuScope export_as_menu {language.get(StringID::kExportAsMenu)};
      export_as_menu.is_open()) {
    if (ImGui::MenuItem("Tiled JSON (TMJ)")) {
      // TODO
    }

    if (ImGui::MenuItem("Tiled XML (TMX)")) {
      // TODO
    }

    if (ImGui::MenuItem("Godot Scene")) {
      dispatcher.push<ShowGodotExportDialogEvent>();
    }
  }
}

}  // namespace tactile::ui
