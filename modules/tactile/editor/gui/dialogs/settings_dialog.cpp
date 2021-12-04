#include "settings_dialog.hpp"

#include <imgui.h>
#include <magic_enum.hpp>  // enum_name

#include "core/utils/color_utils.hpp"
#include "editor/events/command_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/checkbox.hpp"
#include "editor/gui/common/combo.hpp"
#include "editor/gui/common/modal.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/gui/themes.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constexpr auto gFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit bool gIsVisible = false;

inline Preferences gSnapshot;  // The original settings when the dialog was opened
inline Preferences gSettings;  // The value of the settings in the GUI

void UpdatePreviewSettings(const Preferences& prefs)
{
  ApplyTheme(ImGui::GetStyle(), prefs.theme);
  ImGui::GetStyle().WindowBorderSize =
      (prefs.flags & Preferences::window_border) ? 1.0f : 0.0f;
}

void ShowBehaviorTab()
{
  if (Scoped::TabItem item{"Behavior"}; item.IsOpen()) {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults")) {
      Prefs::ResetBehaviorPreferences(gSettings);
      UpdatePreviewSettings(gSettings);
    }
    ImGui::Spacing();

    if (bool restore = gSettings.flags & Preferences::restore_last_session;
        Checkbox("Restore last session on startup", &restore)) {
      gSettings.SetFlag(Preferences::restore_last_session, restore);
    }

    if (auto width = gSettings.preferred_tile_width;
        ImGui::DragInt("Preferred tile width", &width, 1.0f, 1, 10'000)) {
      gSettings.preferred_tile_width = width;
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile width when creating maps");
    }

    if (auto height = gSettings.preferred_tile_height;
        ImGui::DragInt("Preferred tile height", &height, 1.0f, 1, 10'000)) {
      gSettings.preferred_tile_height = height;
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip("The suggested tile height when creating maps");
    }

    // TODO "RMB with stamp tool works as eraser"

    if (auto capacity = static_cast<int>(gSettings.command_capacity);
        ImGui::DragInt("Command capacity", &capacity, 1.0f, 10, 1'000)) {
      gSettings.command_capacity = static_cast<usize>(capacity);
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The maximum amount of commands that will be stored on the undo stack");
    }
  }
}

void ShowAppearanceBar()
{
  if (Scoped::TabItem item{"Appearance"}; item.IsOpen()) {
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults")) {
      Prefs::ResetAppearancePreferences(gSettings);
      UpdatePreviewSettings(gSettings);
    }

    ImGui::Spacing();

    if (Scoped::Combo theme{"Theme", magic_enum::enum_name(gSettings.theme).data()};
        theme.IsOpen()) {
      for (auto&& [name, value] : themes) {
        if (ImGui::Selectable(name)) {
          gSettings.theme = value;
          ApplyTheme(ImGui::GetStyle(), value);
        }
      }
    }

    if (auto arr = ColorToArray(gSettings.viewport_background);
        ImGui::ColorEdit3("Viewport background color",
                          arr.data(),
                          ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
      gSettings.viewport_background =
          cen::color::from_norm(arr.at(0), arr.at(1), arr.at(2));
    }

    if (bool enabled = gSettings.flags & Preferences::window_border;
        ImGui::Checkbox("Window border", &enabled)) {
      gSettings.SetFlag(Preferences::window_border, enabled);
      ImGui::GetStyle().WindowBorderSize = enabled ? 1.0f : 0.0f;
    }

    if (bool restoreLayout = gSettings.flags & Preferences::restore_layout;
        Checkbox("Restore layout",
                 &restoreLayout,
                 "Restore the previous layout of widgets at startup")) {
      gSettings.SetFlag(Preferences::restore_layout, restoreLayout);
    }
  }
}

void ShowExportTab()
{
  if (Scoped::TabItem item{"Export"}; item.IsOpen()) {
    ImGui::Spacing();

    if (ImGui::Button("Restore Defaults")) {
      Prefs::ResetExportPreferences(gSettings);
      UpdatePreviewSettings(gSettings);
    }

    ImGui::Spacing();

    if (Scoped::Combo format("Preferred Format", gSettings.preferred_format.c_str());
        format.IsOpen()) {
      if (ImGui::MenuItem("YAML")) {
        gSettings.preferred_format = "YAML";
      }

      if (ImGui::MenuItem("JSON")) {
        gSettings.preferred_format = "JSON";
      }

      if (ImGui::MenuItem("XML (TMX)")) {
        gSettings.preferred_format = "XML";
      }
    }

    if (ImGui::IsItemHovered()) {
      ImGui::SetTooltip(
          "The save file format used if no file extension is specified when saving maps");
    }

    if (bool embedTilesets = gSettings.flags & Preferences::embed_tilesets;
        Checkbox("Embed tilesets", &embedTilesets, "Embed tileset data in map files")) {
      gSettings.SetFlag(Preferences::embed_tilesets, embedTilesets);
    }

    if (bool indentOutput = gSettings.flags & Preferences::indent_output;
        Checkbox("Indent output",
                 &indentOutput,
                 "Controls whether or not save files are indented")) {
      gSettings.SetFlag(Preferences::indent_output, indentOutput);
    }

    if (bool foldTileData = gSettings.flags & Preferences::fold_tile_data; Checkbox(
            "Fold tile data",
            &foldTileData,
            "Make tile layer data easier for humans to edit, at the expense of space")) {
      gSettings.SetFlag(Preferences::fold_tile_data, foldTileData);
    }
  }
}

void ApplySettings(entt::dispatcher& dispatcher)
{
  SetPreferences(gSettings);
  if (gSettings.command_capacity != gSnapshot.command_capacity) {
    dispatcher.enqueue<SetCommandCapacityEvent>(Prefs::GetCommandCapacity());
  }
}

}  // namespace

void UpdateSettingsDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  Modal modal{"Settings", gFlags, &gIsVisible};
  if (modal.IsOpen()) {
    if (Scoped::TabBar bar{"SettingsTabBar"}; bar.IsOpen()) {
      ShowBehaviorTab();
      ShowAppearanceBar();
      ShowExportTab();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("OK")) {
      ApplySettings(dispatcher);
      UpdatePreviewSettings(GetPreferences());
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      UpdatePreviewSettings(GetPreferences());
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Apply")) {
      ApplySettings(dispatcher);
      UpdatePreviewSettings(GetPreferences());
    }
  }
}

void OpenSettingsDialog()
{
  gIsVisible = true;
  gSnapshot = GetPreferences();
  gSettings = gSnapshot;
  ImGui::OpenPopup("Settings");
}

}  // namespace Tactile
