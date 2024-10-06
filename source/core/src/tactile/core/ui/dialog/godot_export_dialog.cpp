// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dialog/godot_export_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/map_events.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/platform/filesystem.hpp"
#include "tactile/core/ui/common/attribute_widgets.hpp"
#include "tactile/core/ui/common/dialogs.hpp"
#include "tactile/core/ui/common/popups.hpp"
#include "tactile/core/ui/common/style.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {
namespace {

void _push_godot_version_combo(int& version)
{
  constexpr const char* godot3_label = "Godot 3";
  constexpr const char* godot4_label = "Godot 4";
  const char* preview_label = version == 3 ? godot3_label : version == 4 ? godot4_label : "?";

  if (ImGui::BeginCombo("##Version", preview_label)) {
    if (ImGui::Selectable(godot3_label)) {
      version = 3;
    }

    if (const DisabledScope disabled {}; ImGui::Selectable(godot4_label)) {
      version = 4;
    }

    ImGui::EndCombo();
  }
}

}  // namespace

void GodotExportDialog::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const auto* dialog_name = language.get(StringID::kGodotExportDialog);

  center_next_window(ImGuiCond_Always);

  if (const PopupScope dialog {kModalPopup, dialog_name, ImGuiWindowFlags_AlwaysAutoResize};
      dialog.is_open()) {
    const auto* version_label = language.get(StringID::kVersion);
    const auto* project_dir_label = language.get(StringID::kProjectDir);

    const auto label_offset = get_alignment_offset(version_label, project_dir_label);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(version_label);
    ImGui::SameLine(label_offset);
    _push_godot_version_combo(m_version);

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(project_dir_label);
    ImGui::SameLine(label_offset);
    if (auto project_dir = push_directory_input("##Dir", m_project_dir)) {
      m_project_dir = std::move(*project_dir);
    }

    const auto status = push_dialog_control_buttons(language.get(StringID::kCancel),
                                                    language.get(StringID::kSave));
    if (status == DialogStatus::kAccepted) {
      dispatcher.push<ExportAsGodotSceneEvent>(m_version, std::move(m_project_dir));
      m_project_dir.clear();
    }
  }

  if (m_should_open) {
    ImGui::OpenPopup(dialog_name, ImGuiPopupFlags_NoReopen);
    m_should_open = false;
  }
}

void GodotExportDialog::open()
{
  m_project_dir = get_user_home_directory().value_or(".");
  m_version = 3;
  m_should_open = true;
}

}  // namespace tactile::ui
