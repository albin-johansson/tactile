#include "component_editor.hpp"

#include <string>   // string
#include <utility>  // move

#include <imgui.h>

#include "core/components/component.hpp"
#include "core/systems/component_system.hpp"
#include "dialogs/create_component_attribute_dialog.hpp"
#include "dialogs/create_component_dialog.hpp"
#include "dialogs/rename_component_attribute_dialog.hpp"
#include "dialogs/rename_component_dialog.hpp"
#include "editor/events/component_events.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/common/input_widgets.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/properties/dialogs/property_type_combo.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"

namespace tactile {

struct component_editor::component_editor_data final
{
  CreateComponentDialog create_component;
  CreateComponentAttributeDialog create_component_attr;
  RenameComponentDialog rename_component;
  RenameComponentAttributeDialog rename_component_attr;
  maybe<component_id> active_component;
};

component_editor::component_editor()
    : ADialog{"Component Editor"}
    , mData{std::make_unique<component_editor_data>()}
{
  SetAcceptButtonLabel(nullptr);
  SetCloseButtonLabel("Close");
}

component_editor::~component_editor() noexcept = default;

void component_editor::Open(const Model& model)
{
  const auto& registry = model.GetActiveRegistryRef();
  mData->active_component = sys::get_first_available_component_def(registry);
  Show();
}

void component_editor::UpdateContents(const Model& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.GetActiveRegistryRef();
  auto& data = *mData;

  /* Ensure that the active component ID hasn't been invalidated */
  if (data.active_component &&
      !sys::is_valid_component(registry, *data.active_component)) {
    data.active_component.reset();
  }

  if (registry.storage<comp::component_def>().empty()) {
    ImGui::TextUnformatted("There are no available components for the current map.");

    if (centered_button(TAC_ICON_ADD, "Create Component")) {
      data.create_component.Open();
    }
  }
  else {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Component:");
    ImGui::SameLine();

    if (!data.active_component) {
      const auto entity = registry.view<comp::component_def>().front();
      data.active_component = registry.get<comp::component_def>(entity).id;
    }

    const auto& name =
        sys::get_component_def_name(registry, data.active_component.value());
    if (scoped::combo combo{"##ComponentEditorCombo", name.c_str()}; combo.is_open()) {
      for (auto&& [entity, component] : registry.view<comp::component_def>().each()) {
        if (ImGui::Selectable(component.name.c_str())) {
          data.active_component = component.id;
        }
      }
    }

    ImGui::SameLine();

    if (button(TAC_ICON_ADD, "Create Component")) {
      data.create_component.Open();
    }

    ImGui::SameLine();

    if (button(TAC_ICON_THREE_DOTS, "Show Component Actions")) {
      ImGui::OpenPopup("##ComponentEditorPopup");
    }

    show_component_combo_popup(registry, dispatcher);

    ImGui::Separator();
  }

  if (data.active_component) {
    show_component_attributes(registry, dispatcher, *data.active_component);
  }

  data.create_component.Update(model, dispatcher);
  data.create_component_attr.Update(model, dispatcher);
  data.rename_component.Update(model, dispatcher);
  data.rename_component_attr.Update(model, dispatcher);

  ImGui::Spacing();
  ImGui::Separator();
}

void component_editor::show_component_combo_popup(const entt::registry& registry,
                                                  entt::dispatcher& dispatcher)
{
  auto& data = *mData;
  if (scoped::popup popup{"##ComponentEditorPopup"}; popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Component")) {
      const auto id = data.active_component.value();
      data.rename_component.Open(sys::get_component_def_name(registry, id), id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Component")) {
      dispatcher.enqueue<remove_component_def_event>(data.active_component.value());
      data.active_component.reset();
    }
  }
}

void component_editor::show_component_attributes(const entt::registry& registry,
                                                 entt::dispatcher& dispatcher,
                                                 component_id id)
{
  auto& data = *mData;
  const auto& [defEntity, def] = sys::get_component_def(registry, id);

  if (def.attributes.empty()) {
    centered_text("There are no attributes defined for the current component.");
  }
  else {
    constexpr auto table_flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;
    if (scoped::table table{"##ComponentAttributeTable", 3, table_flags};
        table.is_open()) {
      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Default", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableHeadersRow();

      for (const auto& [name, attr] : def.attributes) {
        show_component_attribute(dispatcher, id, name, attr);
      }
    }
  }

  if (centered_button("Create Attribute")) {
    data.create_component_attr.Open(*data.active_component);
  }
}

void component_editor::show_component_attribute(entt::dispatcher& dispatcher,
                                                component_id id,
                                                const std::string& name,
                                                const attribute_value& value)
{
  auto& data = *mData;
  const scoped::id scope{name.c_str()};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(name.c_str());

  if (auto popup = scoped::popup::for_item("##ComponentAttributeNameContext");
      popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Attribute")) {
      data.rename_component_attr.Open(name, data.active_component.value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Attribute")) {
      dispatcher.enqueue<duplicate_component_attr_event>(id, name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Attribute")) {
      dispatcher.enqueue<remove_component_attr_event>(id, name);
    }
  }

  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-min_float);

  const auto type = value.type();
  attribute_type newType = type;
  PropertyTypeCombo(type, newType);
  if (newType != type) {
    dispatcher.enqueue<set_component_attr_type_event>(id, name, newType);
  }

  ImGui::TableNextColumn();

  if (auto updated = input_attribute("##DefaultValue", value)) {
    dispatcher.enqueue<update_component_def_attr_event>(id, name, std::move(*updated));
  }
}

}  // namespace tactile
