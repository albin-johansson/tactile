// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dialog/new_property_dialog.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "tactile/base/container/lookup.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/events.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/attribute_widgets.hpp"
#include "tactile/core/ui/common/dialogs.hpp"
#include "tactile/core/ui/common/popups.hpp"
#include "tactile/core/ui/common/style.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::core::ui {

NewPropertyDialog::NewPropertyDialog()
{
  mName.reserve(128);
}

void NewPropertyDialog::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto* document = model.get_current_document();
  if (!document) {
    return;
  }

  const auto& registry = document->get_registry();
  const auto& language = model.get_language();

  const auto* dialog_name = language.get(ActionLabel::kCreateProperty);

  if (const PopupScope dialog {kModalPopup, dialog_name, ImGuiWindowFlags_AlwaysAutoResize};
      dialog.is_open()) {
    const auto* name_str = language.get(NounLabel::kName);
    const auto* type_str = language.get(NounLabel::kType);
    const auto label_alignment_offset = get_alignment_offset(name_str, type_str);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(name_str);
    ImGui::SameLine(label_alignment_offset);
    ImGui::InputText("##Name", &mName);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(type_str);
    ImGui::SameLine(label_alignment_offset);
    if (auto type = mValue.get_type(); push_attribute_type_combo(language, "##Type", type)) {
      mValue.reset(type);
    }

    const auto& meta = registry.get<CMeta>(mContextEntity);
    const auto can_accept = !mName.empty() && !exists_in(meta.properties, mName);

    const auto action = push_dialog_control_buttons(language.get(VerbLabel::kCancel),
                                                    language.get(VerbLabel::kCreate),
                                                    nullptr,
                                                    can_accept);

    if (action == DialogStatus::kAccepted) {
      dispatcher.push<CreatePropertyEvent>(mContextEntity, mName, mValue);
    }
  }

  if (mShouldOpen) {
    ImGui::OpenPopup(dialog_name, ImGuiPopupFlags_NoReopen);
    mShouldOpen = false;
  }
}

void NewPropertyDialog::open(const EntityID context_entity)
{
  mContextEntity = context_entity;
  mName.clear();
  mValue.reset(AttributeType::kStr);
  mShouldOpen = true;
}

}  // namespace tactile::core::ui
