#include "about_dialog.hpp"

#include <centurion.hpp>
#include <imgui.h>

#include "editor/gui/common/button.hpp"
#include "editor/gui/icons.hpp"

#define TAC_VER "0.3.0"
#define TAC_URL "https://www.github.com/albin-johansson/tactile"
#define TAC_LICENSE "GPL v3.0"

namespace tactile {

about_dialog::about_dialog() : dialog_base{"About Tactile"}
{
  set_accept_button_label(nullptr);
  set_close_button_label("Close");
}

void about_dialog::show()
{
  make_visible();
}

void about_dialog::on_update(const Model&, entt::dispatcher&)
{
  ImGui::TextUnformatted("Tactile " TAC_VER " (C) Albin Johansson 2020-2022");
  ImGui::Separator();

  ImGui::TextUnformatted("This tool is open-source software, using the " TAC_LICENSE
                         " license.");

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Source code: " TAC_URL);
  ImGui::SameLine();
  if (button(TAC_ICON_LINK, "Open the GitHub repository in your browser")) {
    cen::open_url(TAC_URL);
  }

  ImGui::Spacing();
  ImGui::TextUnformatted("Icons by Font Awesome.");
}

}  // namespace tactile
