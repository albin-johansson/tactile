// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dialog/rename_property_dialog.hpp"

#include <utility>  // move

#include <imgui.h>
#include <imgui_stdlib.h>

#include "tactile/base/document/document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/property_events.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/dialogs.hpp"
#include "tactile/core/ui/common/popups.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile::core::ui {

RenamePropertyDialog::RenamePropertyDialog()
{
  mNewPropName.reserve(128);
}

void RenamePropertyDialog::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto* document = model.get_current_document();
  if (!document) {
    return;
  }

  const auto& registry = document->get_registry();
  const auto& language = model.get_language();

  const auto* dialog_name = language.get(StringID::kRenameProperty);

  if (const PopupScope dialog {kModalPopup, dialog_name, ImGuiWindowFlags_AlwaysAutoResize};
      dialog.is_open()) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(language.get(StringID::kName));
    ImGui::SameLine();
    ImGui::InputText("##Name", &mNewPropName);

    const auto& meta = registry.get<CMeta>(mContextEntity);
    const auto can_accept = !mNewPropName.empty() && !exists_in(meta.properties, mNewPropName);

    const auto action = push_dialog_control_buttons(language.get(StringID::kCancel),
                                                    language.get(StringID::kRename),
                                                    nullptr,
                                                    can_accept);

    if (action == DialogStatus::kAccepted) {
      dispatcher.push<RenamePropertyEvent>(mContextEntity, mTargetPropName, mNewPropName);
    }
  }

  if (mShouldOpen) {
    ImGui::OpenPopup(dialog_name, ImGuiPopupFlags_NoReopen);
    mShouldOpen = false;
  }
}

void RenamePropertyDialog::open(const EntityID context_entity, std::string target_prop_name)
{
  mContextEntity = context_entity;
  mTargetPropName = std::move(target_prop_name);

  mNewPropName.clear();
  mNewPropName = mTargetPropName;

  mShouldOpen = true;
}

}  // namespace tactile::core::ui
