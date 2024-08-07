// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/layer_dock.hpp"

#include <imgui.h>

#include "tactile/base/util/buffer.hpp"
#include "tactile/base/util/format.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/layer_events.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/core/ui/common/icons.hpp"
#include "tactile/core/ui/common/popups.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {
namespace layer_dock {

using NameBuffer = Buffer<char, 128>;

inline constexpr const char* kNewLayerPopupId = "##NewLayerPopup";

void push_new_layer_popup(const Language& language, EventDispatcher& dispatcher)
{
  if (const PopupScope new_layer_popup {kNewLayerPopupId}; new_layer_popup.is_open()) {
    if (ImGui::MenuItem(language.get(StringID::kTileLayerItem))) {
      dispatcher.push<CreateLayerEvent>(LayerType::kTileLayer);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kObjectLayerItem))) {
      dispatcher.push<CreateLayerEvent>(LayerType::kObjectLayer);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kGroupLayerItem))) {
      dispatcher.push<CreateLayerEvent>(LayerType::kGroupLayer);
    }
  }
}

void push_tile_layer_item(const char* name)
{
  NameBuffer name_buffer;
  format_to_buffer(name_buffer, "{} {}", to_string(Icon::kTileLayer), name);
  name_buffer.set_terminator('\0');

  if (ImGui::TreeNodeEx(name_buffer.data(),
                        ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed |
                            ImGuiTreeNodeFlags_FramePadding)) {
    ImGui::TreePop();
  }
}

void push_object_layer_item(const char* name)
{
  NameBuffer name_buffer;
  format_to_buffer(name_buffer, "{} {}", to_string(Icon::kObjectLayer), name);
  name_buffer.set_terminator('\0');

  if (ImGui::TreeNodeEx(name_buffer.data(),
                        ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed |
                            ImGuiTreeNodeFlags_FramePadding)) {
    ImGui::TreePop();
  }
}

void push_layer_item(const Language& language,
                     const Registry& registry,
                     const EntityID layer_id,
                     const bool top_level = false)
{
  const IdScope layer_scope {layer_id};

  const auto& meta = registry.get<CMeta>(layer_id);

  if (!top_level) {
    ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
  }

  if (registry.has<CGroupLayer>(layer_id)) {
    if (ImGui::TreeNodeEx("##Group",
                          ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding,
                          "%s",
                          meta.name.c_str())) {
      const IndentScope indent_scope {};

      const auto& group_layer = registry.get<CGroupLayer>(layer_id);
      for (const auto& nested_layer_id : group_layer.layers) {
        push_layer_item(language, registry, nested_layer_id);
      }

      ImGui::TreePop();
    }
  }
  else if (registry.has<CObjectLayer>(layer_id)) {
    push_object_layer_item(meta.name.c_str());
  }
  else if (registry.has<CTileLayer>(layer_id)) {
    push_tile_layer_item(meta.name.c_str());
  }

  if (!top_level) {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
  }
}

void push_layer_list_box(const Language& language,
                         const Registry& registry,
                         const CGroupLayer& root_layer)
{
  if (ImGui::BeginListBox("##LayerListBox", ImVec2 {-1.0f, -1.0f})) {
    for (const auto layer_id : root_layer.layers) {
      push_layer_item(language, registry, layer_id, true);
    }

    ImGui::EndListBox();
  }
}

void push_layer_group(const Language& language, const Registry& registry, const CMap& map)
{
  const GroupScope layer_group {};

  const auto& root_layer = registry.get<CGroupLayer>(map.root_layer);

  if (root_layer.layers.empty()) {
    prepare_for_vertically_centered_widgets(2);
    push_centered_label(language.get(StringID::kMapHasNoLayers));
    if (push_horizontally_centered_button(language.get(StringID::kCreateLayer))) {
      ImGui::OpenPopup(kNewLayerPopupId, ImGuiPopupFlags_NoReopen);
    }
  }
  else {
    push_layer_list_box(language, registry, root_layer);
  }
}

void push_side_button_group(const Registry& registry,
                            const CMap& map,
                            EventDispatcher& dispatcher)
{
  const GroupScope button_group_scope {};

  const auto has_active_layer = map.active_layer != kInvalidEntity;
  const auto can_move_up =
      has_active_layer && can_move_layer_up(registry, map.root_layer, map.active_layer);
  const auto can_move_down =
      has_active_layer && can_move_layer_down(registry, map.root_layer, map.active_layer);

  if (push_icon_button(Icon::kCreate)) {
    ImGui::OpenPopup(kNewLayerPopupId, ImGuiPopupFlags_NoReopen);
  }

  if (push_icon_button(Icon::kRemove, nullptr, has_active_layer)) {
    dispatcher.push<RemoveLayerEvent>(map.active_layer);
  }

  if (push_icon_button(Icon::kDuplicate, nullptr, has_active_layer)) {
    dispatcher.push<DuplicateLayerEvent>(map.active_layer);
  }

  if (push_icon_button(Icon::kMoveUp, nullptr, can_move_up)) {
    dispatcher.push<MoveLayerUpEvent>(map.active_layer);
  }

  if (push_icon_button(Icon::kMoveDown, nullptr, can_move_down)) {
    dispatcher.push<MoveLayerDownEvent>(map.active_layer);
  }
}

}  // namespace layer_dock

void LayerDock::update(const Language& language,
                       const MapDocument& document,
                       EventDispatcher& dispatcher)
{
  if (const Window window {language.get(StringID::kLayerDock)}; window.is_open()) {
    const auto& registry = document.get_registry();

    const auto& document_info = registry.get<CDocumentInfo>();
    const auto& map = registry.get<CMap>(document_info.root);

    layer_dock::push_side_button_group(registry, map, dispatcher);
    ImGui::SameLine();
    layer_dock::push_layer_group(language, registry, map);

    layer_dock::push_new_layer_popup(language, dispatcher);
  }
}

}  // namespace tactile::ui
