#include "about_dialog.hpp"

#include <centurion.hpp>
#include <imgui.h>

#include "editor/gui/common/button.hpp"
#include "editor/gui/icons.hpp"

#define TACTILE_VER "0.3.0"
#define TACTILE_URL "https://www.github.com/albin-johansson/tactile"
#define TACTILE_LICENSE "GPL v3.0"

namespace tactile {

AboutDialog::AboutDialog() : ADialog{"About Tactile"}
{
  SetAcceptButtonLabel(nullptr);
  SetCloseButtonLabel("Close");
}

void AboutDialog::Open()
{
  Show();
}

void AboutDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  ImGui::TextUnformatted("Tactile " TACTILE_VER " (C) Albin Johansson 2020-2022");
  ImGui::Separator();

  ImGui::TextUnformatted("This tool is open-source software, using the " TACTILE_LICENSE
                         " license.");

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Source code: " TACTILE_URL);
  ImGui::SameLine();
  if (button(TAC_ICON_LINK, "Open the GitHub repository in your browser")) {
    cen::open_url(TACTILE_URL);
  }

  ImGui::Spacing();
  ImGui::TextUnformatted("Icons by Font Awesome.");
}

}  // namespace tactile
