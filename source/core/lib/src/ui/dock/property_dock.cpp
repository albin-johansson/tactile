// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/property_dock.hpp"

#include <utility>  // move

#include <imgui.h>

#include "tactile/base/document/document.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/property_events.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/ui/common/attribute_widgets.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::core::ui {
namespace {

void _push_property_table_context_menu_content(const Language& language,
                                               const EntityID context_entity,
                                               const std::string* prop_name,
                                               EventDispatcher& dispatcher)
{
  if (const DisabledScope disable_if {prop_name != nullptr};
      ImGui::Selectable(language.get(StringID::kCreateProperty))) {
    dispatcher.push<ShowNewPropertyDialogEvent>(context_entity);
  }

  ImGui::Separator();

  {
    const DisabledScope disable_if {prop_name == nullptr};

    if (ImGui::Selectable(language.get(StringID::kRenameProperty))) {
      TACTILE_ASSERT(prop_name != nullptr);
      dispatcher.push<ShowRenamePropertyDialogEvent>(context_entity, *prop_name);
    }

    ImGui::Separator();

    if (ImGui::Selectable(language.get(StringID::kRemoveProperty))) {
      TACTILE_ASSERT(prop_name != nullptr);
      dispatcher.push<RemovePropertyEvent>(context_entity, *prop_name);
    }
  }
}

void _push_property_table(const Language& language,
                          const EntityID context_entity,
                          const CMeta& meta,
                          EventDispatcher& dispatcher)
{
  const auto table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                           ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInner;
  if (const TableScope table {"##Properties", 2, table_flags}; table.is_open()) {
    ImGui::TableSetupColumn(language.get(StringID::kName));
    ImGui::TableSetupColumn(language.get(StringID::kValue));

    for (const auto& [prop_name, prop_value] : meta.properties) {
      const IdScope prop_scope {prop_name.c_str()};

      if (ImGui::TableNextColumn()) {
        ImGui::AlignTextToFramePadding();
        ImGui::Selectable(prop_name.c_str());

        const auto item_popup_flags =
            ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;
        if (ImGui::BeginPopupContextItem(nullptr, item_popup_flags)) {
          _push_property_table_context_menu_content(language,
                                                    context_entity,
                                                    &prop_name,
                                                    dispatcher);
          ImGui::EndPopup();
        }
      }

      if (ImGui::TableNextColumn()) {
        if (auto new_prop_value = push_attribute_input("##Value", prop_value)) {
          dispatcher.push<UpdatePropertyEvent>(context_entity,
                                               prop_name,
                                               std::move(*new_prop_value));
        }
      }
    }

    const auto window_popup_flags = ImGuiPopupFlags_MouseButtonRight |
                                    ImGuiPopupFlags_NoOpenOverExistingPopup |
                                    ImGuiPopupFlags_NoOpenOverItems;
    if (ImGui::BeginPopupContextWindow(nullptr, window_popup_flags)) {
      _push_property_table_context_menu_content(language, context_entity, nullptr, dispatcher);
      ImGui::EndPopup();
    }
  }
}

void _push_no_properties_view(const Language& language,
                              const EntityID context_entity,
                              EventDispatcher& dispatcher)
{
  prepare_for_vertically_centered_widgets(2);

  push_centered_label(language.get(StringID::kContextHasNoProperties));

  if (push_horizontally_centered_button(language.get(StringID::kCreateProperty))) {
    dispatcher.push<ShowNewPropertyDialogEvent>(context_entity);
  }
}

}  // namespace

void PropertyDock::push(const Language& language,
                        const IDocument& document,
                        EventDispatcher& dispatcher)
{
  if (const Window dock_window {language.get(StringID::kPropertyDock)};
      dock_window.is_open()) {
    const auto& registry = document.get_registry();
    const auto& document_info = registry.get<CDocumentInfo>();

    const auto context_entity = document_info.active_context;

    TACTILE_ASSERT(context_entity != kInvalidEntity);
    const auto& meta = registry.get<CMeta>(context_entity);

    if (meta.properties.empty()) {
      _push_no_properties_view(language, context_entity, dispatcher);
    }
    else {
      _push_property_table(language, context_entity, meta, dispatcher);
    }
  }
}

}  // namespace tactile::core::ui
