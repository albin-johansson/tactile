#include "settings_dialog.hpp"

#include <imgui.h>

#include "events/change_command_capacity_event.hpp"
#include "gui/themes.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

inline bool restore_last_session = false;

void ShowGeneralTab(entt::dispatcher& dispatcher)
{
  if (ImGui::BeginTabItem("General"))
  {
    ImGui::Spacing();

    ImGui::TextUnformatted("Startup");
    ImGui::Checkbox("Restore last session", &restore_last_session);

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextUnformatted("Behavior");

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Command capacity:");
    ImGui::SameLine();

    if (auto capacity = static_cast<int>(Prefs::GetCommandCapacity());
        ImGui::DragInt("##CommandCapacityInput", &capacity, 1.0f, 10, 1'000))
    {
      Prefs::SetCommandCapacity(static_cast<usize>(capacity));
      dispatcher.enqueue<ChangeCommandCapacityEvent>(Prefs::GetCommandCapacity());
    }

    if (ImGui::IsItemHovered())
    {
      ImGui::SetTooltip(
          "The maximum amount of commands that will be stored on the undo stack.");
    }

    // ImGui::Checkbox("RMB with stamp tool works as eraser", &rmb_stamp_as_eraser);

    ImGui::EndTabItem();
  }
}

void ShowThemeBar()
{
  if (ImGui::BeginTabItem("Theme"))
  {
    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Theme:");

    ImGui::SameLine();
    if (auto themeIndex = GetThemeIndex(Prefs::GetTheme());
        ImGui::Combo("##ThemeCombo", &themeIndex, theme_options))
    {
      const auto theme = GetThemeFromIndex(themeIndex);
      Prefs::SetTheme(theme);
      ApplyTheme(ImGui::GetStyle(), theme);
    }

    if (bool enabled = Prefs::GetWindowBorder();
        ImGui::Checkbox("Window border", &enabled))
    {
      Prefs::SetWindowBorder(enabled);
      ImGui::GetStyle().WindowBorderSize = enabled ? 1.0f : 0.0f;
    }

    if (bool restoreLayout = Prefs::GetRestoreLayout();
        ImGui::Checkbox("Restore layout", &restoreLayout))
    {
      Prefs::SetRestoreLayout(restoreLayout);
    }

    if (ImGui::IsItemHovered())
    {
      ImGui::SetTooltip("Restore the previous layout of widgets at startup.");
    }

    ImGui::EndTabItem();
  }
}

void ShowExportTab()
{
  if (ImGui::BeginTabItem("Export"))
  {
    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Preferred format:");

    int formatIndex = (Prefs::GetPreferredFormat() == "JSON") ? 0 : 1;
    ImGui::SameLine();
    if (ImGui::Combo("##PreferredFormatCombo", &formatIndex, "JSON\0TMX\0\0"))
    {
      Prefs::SetPreferredFormat((formatIndex == 0) ? "JSON" : "TMX");
    }

    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip("The default save file format.");
    }

    bool embedTilesets = Prefs::GetEmbedTilesets();
    if (ImGui::Checkbox("Embed tilesets", &embedTilesets))
    {
      Prefs::SetEmbedTilesets(embedTilesets);
    }
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip("Embed tileset data in map files.");
    }

    bool humanReadableOutput = Prefs::GetHumanReadableOutput();
    if (ImGui::Checkbox("Human-readable output", &humanReadableOutput))
    {
      Prefs::SetHumanReadableOutput(humanReadableOutput);
    }

    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip(
          "Human-readable saves are easier for humans to process, but take up "
          "more space.");
    }

    ImGui::EndTabItem();
  }
}

}  // namespace

void UpdateSettingsDialog(entt::dispatcher& dispatcher, bool* open)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{500, 300});

  if (ImGui::Begin("Settings", open, ImGuiWindowFlags_NoCollapse))
  {
    if (ImGui::BeginTabBar("SettingsTabBar"))
    {
      ShowGeneralTab(dispatcher);
      ShowThemeBar();
      ShowExportTab();
      ImGui::EndTabBar();
    }
  }

  ImGui::PopStyleVar();
  ImGui::End();
}

}  // namespace Tactile
