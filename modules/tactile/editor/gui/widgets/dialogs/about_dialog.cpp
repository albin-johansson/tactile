#include "about_dialog.hpp"

#include <centurion.hpp>  // open_url
#include <imgui.h>

#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/button.hpp"
#include "editor/gui/widgets/common/window.hpp"

#define TACTILE_VER "0.3.0"
#define TACTILE_URL "https://www.github.com/albin-johansson/tactile"

namespace Tactile {
namespace {

constexpr auto gSourceCode = "Source code: " TACTILE_URL;

}  // namespace

void UpdateAboutDialog(bool* open)
{
  CenterNextWindowOnAppearance();
  auto window = Window{"About Tactile", ImGuiWindowFlags_AlwaysAutoResize, open};
  if (window) {
    ImGui::TextUnformatted("Tactile " TACTILE_VER " (C) Albin Johansson 2020-2021");
    ImGui::Separator();

    ImGui::TextUnformatted(
        "This tool is open-source software, using the GPL v3.0 license.");

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(gSourceCode);
    ImGui::SameLine();
    if (Button(TAC_ICON_LINK, "Open the GitHub repository in your browser")) {
      cen::open_url(TACTILE_URL);
    }

    ImGui::Spacing();
    ImGui::TextUnformatted("Icons by Font Awesome.");
  }
}

}  // namespace Tactile
