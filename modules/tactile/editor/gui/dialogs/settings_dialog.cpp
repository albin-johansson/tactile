#include "settings_dialog.hpp"

#include <imgui.h>
#include <magic_enum.hpp>

#include "core/utils/color_utils.hpp"
#include "editor/events/command_events.hpp"
#include "editor/gui/common/checkbox.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/themes.hpp"
#include "io/preferences.hpp"

namespace tactile {
namespace {

void UpdatePreviewSettings(const Preferences& prefs)
{
  ApplyTheme(ImGui::GetStyle(), prefs.theme);
  ImGui::GetStyle().WindowBorderSize =
      (prefs.flags & Preferences::window_border) ? 1.0f : 0.0f;
}

}  // namespace

SettingsDialog::SettingsDialog() : ADialog{"Settings"}
{
  SetApplyButtonLabel("Apply");
}

void SettingsDialog::Open()
{
  mSnapshot = GetPreferences();
  mSettings = mSnapshot;
  Show();
}

void SettingsDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  Scoped::TabBar bar{"##SettingsTabBar"};
  if (bar.IsOpen()) {
    UpdateBehaviorTab();
    UpdateAppearanceTab();
    UpdateExportTab();
  }
}

void SettingsDialog::OnCancel()
{
  UpdatePreviewSettings(GetPreferences());
}

void SettingsDialog::OnAccept(entt::dispatcher& dispatcher)
{
  ApplySettings(dispatcher);
  UpdatePreviewSettings(GetPreferences());
}

void SettingsDialog::OnApply(entt::dispatcher& dispatcher)
{
  ApplySettings(dispatcher);
  UpdatePreviewSettings(GetPreferences());
}

void SettingsDialog::ApplySettings(entt::dispatcher& dispatcher)
{
  SetPreferences(mSettings);
  if (mSettings.command_capacity != mSnapshot.command_capacity) {
    dispatcher.enqueue<SetCommandCapacityEvent>(prefs::GetCommandCapacity());
  }
}

void SettingsDialog::UpdateBehaviorTab()
{
  if (Scoped::TabItem item{"Behavior"}; item.IsOpen()) {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults")) {
      prefs::ResetBehaviorPreferences(mSettings);
      UpdatePreviewSettings(mSettings);
    }
    ImGui::Spacing();

    if (bool restore = mSettings.flags & Preferences::restore_last_session;
        Checkbox("Restore last session on startup", &restore)) {
      mSettings.SetFlag(Preferences::restore_last_session, restore);
    }

    if (auto width = mSettings.preferred_tile_width;
        ImGui::DragInt("Preferred tile width", &width, 1.0f, 1, 10'000)) {
      mSettings.preferred_tile_width = width;
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile width when creating maps");
    }

    if (auto height = mSettings.preferred_tile_height;
        ImGui::DragInt("Preferred tile height", &height, 1.0f, 1, 10'000)) {
      mSettings.preferred_tile_height = height;
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile height when creating maps");
    }

    // TODO "RMB with stamp tool works as eraser"

    if (auto capacity = static_cast<int>(mSettings.command_capacity);
        ImGui::DragInt("Command capacity", &capacity, 1.0f, 10, 1'000)) {
      mSettings.command_capacity = static_cast<usize>(capacity);
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The maximum amount of commands that will be stored on the undo stack");
    }
  }
}

void SettingsDialog::UpdateAppearanceTab()
{
  if (Scoped::TabItem item{"Appearance"}; item.IsOpen()) {
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults")) {
      prefs::ResetAppearancePreferences(mSettings);
      UpdatePreviewSettings(mSettings);
    }

    ImGui::Spacing();

    if (Scoped::Combo theme{"Theme", magic_enum::enum_name(mSettings.theme).data()};
        theme.IsOpen()) {
      for (auto&& [name, value] : themes) {
        if (ImGui::Selectable(name)) {
          mSettings.theme = value;
          ApplyTheme(ImGui::GetStyle(), value);
        }
      }
    }

    if (auto arr = ColorToArray(mSettings.viewport_background);
        ImGui::ColorEdit3("Viewport background color",
                          arr.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
      mSettings.viewport_background =
          cen::color::from_norm(arr.at(0), arr.at(1), arr.at(2));
    }

    if (bool enabled = mSettings.flags & Preferences::window_border;
        ImGui::Checkbox("Window border", &enabled)) {
      mSettings.SetFlag(Preferences::window_border, enabled);
      ImGui::GetStyle().WindowBorderSize = enabled ? 1.0f : 0.0f;
    }

    if (bool restoreLayout = mSettings.flags & Preferences::restore_layout;
        Checkbox("Restore layout",
                 &restoreLayout,
                 "Restore the previous layout of widgets at startup")) {
      mSettings.SetFlag(Preferences::restore_layout, restoreLayout);
    }
  }
}

void SettingsDialog::UpdateExportTab()
{
  if (Scoped::TabItem item{"Export"}; item.IsOpen()) {
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults")) {
      prefs::ResetExportPreferences(mSettings);
      UpdatePreviewSettings(mSettings);
    }

    ImGui::Spacing();

    if (Scoped::Combo format("Preferred Format", mSettings.preferred_format.c_str());
        format.IsOpen()) {
      if (ImGui::MenuItem("YAML")) {
        mSettings.preferred_format = "YAML";
      }

      if (ImGui::MenuItem("JSON")) {
        mSettings.preferred_format = "JSON";
      }

      if (ImGui::MenuItem("XML (TMX)")) {
        mSettings.preferred_format = "XML";
      }
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The save file format used if no file extension is specified when saving maps");
    }

    if (bool embedTilesets = mSettings.flags & Preferences::embed_tilesets;
        Checkbox("Embed tilesets", &embedTilesets, "Embed tileset data in map files")) {
      mSettings.SetFlag(Preferences::embed_tilesets, embedTilesets);
    }

    if (bool indentOutput = mSettings.flags & Preferences::indent_output;
        Checkbox("Indent output",
                 &indentOutput,
                 "Controls whether or not save files are indented")) {
      mSettings.SetFlag(Preferences::indent_output, indentOutput);
    }

    if (bool foldTileData = mSettings.flags & Preferences::fold_tile_data; Checkbox(
            "Fold tile data",
            &foldTileData,
            "Make tile layer data easier for humans to edit, at the expense of space")) {
      mSettings.SetFlag(Preferences::fold_tile_data, foldTileData);
    }
  }
}

}  // namespace tactile
