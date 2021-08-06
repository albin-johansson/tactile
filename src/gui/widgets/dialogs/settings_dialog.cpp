#include "settings_dialog.hpp"

#include <imgui.h>

#include "events/change_command_capacity_event.hpp"
#include "gui/themes.hpp"
#include "gui/widgets/common/checkbox.hpp"
#include "gui/widgets/common/combo.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constexpr auto flags =
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

inline Preferences snapshot;  // The original settings when the dialog was opened
inline Preferences settings;  // The value of the settings in the GUI

void UpdatePreviewSettings(const Preferences& prefs)
{
  ApplyTheme(ImGui::GetStyle(), prefs.theme);
  ImGui::GetStyle().WindowBorderSize = prefs.window_border ? 1.0f : 0.0f;
}

void ShowBehaviorTab(entt::dispatcher& dispatcher)
{
  if (ImGui::BeginTabItem("Behavior"))
  {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults"))
    {
      Prefs::ResetBehaviorPreferences(settings);
      UpdatePreviewSettings(settings);
    }
    ImGui::Spacing();

    if (auto restore = settings.restore_last_session;
        Checkbox("Restore last session on startup", &restore))
    {
      settings.restore_last_session = restore;
    }

    // TODO "RMB with stamp tool works as eraser"

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Command capacity:");
    ImGui::SameLine();

    if (auto capacity = static_cast<int>(settings.command_capacity);
        ImGui::DragInt("##CommandCapacityInput", &capacity, 1.0f, 10, 1'000))
    {
      settings.command_capacity = static_cast<usize>(capacity);
    }

    if (ImGui::IsItemHovered())
    {
      ImGui::SetTooltip(
          "The maximum amount of commands that will be stored on the undo stack.");
    }

    ImGui::EndTabItem();
  }
}

void ShowAppearanceBar()
{
  if (ImGui::BeginTabItem("Appearance"))
  {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults"))
    {
      Prefs::ResetAppearancePreferences(settings);
      UpdatePreviewSettings(settings);
    }
    ImGui::Spacing();

    if (auto index = GetThemeIndex(settings.theme);
        Combo("Theme:", theme_options, &index))
    {
      settings.theme = GetThemeFromIndex(index);
      ApplyTheme(ImGui::GetStyle(), settings.theme);
    }

    if (auto enabled = settings.window_border;
        ImGui::Checkbox("Window border", &enabled))
    {
      settings.window_border = enabled;
      ImGui::GetStyle().WindowBorderSize = enabled ? 1.0f : 0.0f;
    }

    if (auto restoreLayout = settings.restore_layout;
        Checkbox("Restore layout",
                 &restoreLayout,
                 "Restore the previous layout of widgets at startup."))
    {
      settings.restore_layout = restoreLayout;
    }

    ImGui::EndTabItem();
  }
}

void ShowExportTab()
{
  if (ImGui::BeginTabItem("Export"))
  {
    ImGui::Spacing();
    if (ImGui::Button("Restore Defaults"))
    {
      Prefs::ResetExportPreferences(settings);
      UpdatePreviewSettings(settings);
    }
    ImGui::Spacing();

    if (auto index = (settings.preferred_format == "JSON") ? 0 : 1;
        Combo("Preferred format:",
              "JSON\0TMX\0\0",
              &index,
              "The default save file format."))
    {
      settings.preferred_format = (index == 0) ? "JSON" : "TMX";
    }

    if (auto embedTilesets = settings.embed_tilesets;
        Checkbox("Embed tilesets",
                 &embedTilesets,
                 "Embed tileset data in map files."))
    {
      settings.embed_tilesets = embedTilesets;
    }

    if (auto humanReadableOutput = settings.human_readable_output;
        Checkbox("Human-readable output",
                 &humanReadableOutput,
                 "Make save files easier for humans to edit, at the cost of space."))
    {
      settings.human_readable_output = humanReadableOutput;
    }

    ImGui::EndTabItem();
  }
}

void ApplySettings(entt::dispatcher& dispatcher)
{
  SetPreferences(settings);
  if (settings.command_capacity != snapshot.command_capacity)
  {
    dispatcher.enqueue<ChangeCommandCapacityEvent>(Prefs::GetCommandCapacity());
  }
}

}  // namespace

void UpdateSettingsDialog(entt::dispatcher& dispatcher)
{
  if (ImGui::BeginPopupModal("Settings", nullptr, flags))
  {
    if (ImGui::BeginTabBar("SettingsTabBar"))
    {
      ShowBehaviorTab(dispatcher);
      ShowAppearanceBar();
      ShowExportTab();
      ImGui::EndTabBar();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("OK"))
    {
      ApplySettings(dispatcher);
      UpdatePreviewSettings(GetPreferences());
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      UpdatePreviewSettings(GetPreferences());
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Apply"))
    {
      ApplySettings(dispatcher);
      UpdatePreviewSettings(GetPreferences());
    }

    ImGui::EndPopup();
  }
}

void OpenSettingsDialog()
{
  snapshot = GetPreferences();
  settings = snapshot;
  ImGui::OpenPopup("Settings");
}

}  // namespace Tactile
