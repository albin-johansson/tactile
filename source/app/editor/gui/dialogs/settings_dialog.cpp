#include "settings_dialog.hpp"

#include <imgui.h>

#include "core/utils/colors.hpp"
#include "editor/events/command_events.hpp"
#include "editor/gui/common/checkbox.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/themes.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {
namespace {

void UpdatePreviewSettings(const preference_state& prefs)
{
  apply_theme(ImGui::GetStyle(), prefs.get_theme());
  ImGui::GetStyle().WindowBorderSize = prefs.has_window_border() ? 1.0f : 0.0f;
}

}  // namespace

SettingsDialog::SettingsDialog() : ADialog{"Settings"}
{
  SetApplyButtonLabel("Apply");
}

void SettingsDialog::Open()
{
  mSnapshot = get_preferences();
  mGuiSettings = mSnapshot;
  Show();
}

void SettingsDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  scoped::tab_bar bar{"##SettingsTabBar"};
  if (bar.is_open()) {
    UpdateBehaviorTab();
    UpdateAppearanceTab();
    UpdateExportTab();
  }
}

void SettingsDialog::OnCancel()
{
  UpdatePreviewSettings(get_preferences());
}

void SettingsDialog::OnAccept(entt::dispatcher& dispatcher)
{
  ApplySettings(dispatcher);
  UpdatePreviewSettings(get_preferences());
}

void SettingsDialog::OnApply(entt::dispatcher& dispatcher)
{
  ApplySettings(dispatcher);
  UpdatePreviewSettings(get_preferences());
}

void SettingsDialog::ApplySettings(entt::dispatcher& dispatcher)
{
  set_preferences(mGuiSettings);
  if (mGuiSettings.command_capacity() != mSnapshot.command_capacity()) {
    dispatcher.enqueue<set_command_capacity_event>(mGuiSettings.command_capacity());
  }
}

void SettingsDialog::UpdateBehaviorTab()
{
  if (scoped::tab_item item{"Behavior"}; item.is_open()) {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults")) {
      mGuiSettings.reset_behavior_preferences();
      UpdatePreviewSettings(mGuiSettings);
    }
    ImGui::Spacing();

    if (bool restore = mGuiSettings.will_restore_last_session();
        Checkbox("Restore last session on startup", &restore)) {
      mGuiSettings.set_will_restore_last_session(restore);
    }

    if (auto width = mGuiSettings.preferred_tile_width();
        ImGui::DragInt("Preferred tile width", &width, 1.0f, 1, 10'000)) {
      mGuiSettings.set_preferred_tile_width(width);
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile width when creating maps");
    }

    if (auto height = mGuiSettings.preferred_tile_height();
        ImGui::DragInt("Preferred tile height", &height, 1.0f, 1, 10'000)) {
      mGuiSettings.set_preferred_tile_height(height);
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile height when creating maps");
    }

    // TODO "RMB with stamp tool works as eraser"

    if (auto capacity = static_cast<int>(mGuiSettings.command_capacity());
        ImGui::DragInt("Command capacity", &capacity, 1.0f, 10, 1'000)) {
      mGuiSettings.set_command_capacity(static_cast<usize>(capacity));
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The maximum amount of commands that will be stored on the undo stack");
    }
  }
}

void SettingsDialog::UpdateAppearanceTab()
{
  if (scoped::tab_item item{"Appearance"}; item.is_open()) {
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults")) {
      mGuiSettings.reset_appearance_preferences();
      UpdatePreviewSettings(mGuiSettings);
    }

    ImGui::Spacing();

    if (scoped::combo combo{"Theme",
                            human_readable_name(mGuiSettings.get_theme()).data()};
        combo.is_open()) {
      for (const auto theme : themes) {
        if (ImGui::Selectable(human_readable_name(theme).data())) {
          mGuiSettings.set_theme(theme);
          apply_theme(ImGui::GetStyle(), theme);
        }
      }
    }

    if (auto arr = color_to_array(mGuiSettings.viewport_bg());
        ImGui::ColorEdit3("Viewport background color",
                          arr.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
      mGuiSettings.set_viewport_bg(
          cen::color::from_norm(arr.at(0), arr.at(1), arr.at(2)));
    }

    if (bool enabled = mGuiSettings.has_window_border();
        ImGui::Checkbox("Window border", &enabled)) {
      mGuiSettings.set_window_border(enabled);
      ImGui::GetStyle().WindowBorderSize = enabled ? 1.0f : 0.0f;
    }

    if (bool restore = mGuiSettings.will_restore_layout();
        Checkbox("Restore layout",
                 &restore,
                 "Restore the previous layout of widgets at startup")) {
      mGuiSettings.set_will_restore_layout(restore);
    }
  }
}

void SettingsDialog::UpdateExportTab()
{
  if (scoped::tab_item item{"Export"}; item.is_open()) {
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults")) {
      mGuiSettings.reset_export_preferences();
      UpdatePreviewSettings(mGuiSettings);
    }

    ImGui::Spacing();

    if (scoped::combo format("Preferred Format", mGuiSettings.preferred_format().c_str());
        format.is_open()) {
      if (ImGui::MenuItem("YAML")) {
        mGuiSettings.set_preferred_format("YAML");
      }

      if (ImGui::MenuItem("JSON")) {
        mGuiSettings.set_preferred_format("JSON");
      }

      if (ImGui::MenuItem("XML (TMX)")) {
        mGuiSettings.set_preferred_format("XML");
      }
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The save file format used if no file extension is specified when saving maps");
    }

    if (bool embed = mGuiSettings.embed_tilesets();
        Checkbox("Embed tilesets", &embed, "Embed tileset data in map files")) {
      mGuiSettings.set_embed_tilesets(embed);
    }

    if (bool indent = mGuiSettings.indent_output();
        Checkbox("Indent output",
                 &indent,
                 "Controls whether or not save files are indented")) {
      mGuiSettings.set_indent_output(indent);
    }

    if (bool fold = mGuiSettings.fold_tile_data();  //
        Checkbox(
            "Fold tile data",
            &fold,
            "Make tile layer data easier for humans to edit, at the expense of space")) {
      mGuiSettings.set_fold_tile_data(fold);
    }
  }
}

}  // namespace tactile
