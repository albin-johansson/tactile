// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "about_dialog.hpp"

#include <centurion/system.hpp>
#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/base/prelude.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

inline constinit bool gOpenDialog = false;

}  // namespace

void open_about_dialog()
{
  gOpenDialog = true;
}

void update_about_dialog()
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.about_tactile.c_str(),
      .accept_label = lang.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (gOpenDialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gOpenDialog = false;
  }

  if (const ScopedDialog dialog {options}; dialog.was_opened()) {
    ImGui::TextUnformatted("Tactile " TACTILE_VERSION_STRING
                           " (C) Albin Johansson 2020-2022");

    ImGui::Separator();
    ImGui::TextUnformatted(lang.misc.license_info.c_str());

    ImGui::Spacing();
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.misc.repository_link.c_str());

    ImGui::SameLine();
    if (ui_button(TAC_ICON_LINK, lang.tooltip.repository_link.c_str())) {
      cen::open_url("https://www.github.com/albin-johansson/tactile");
    }

    ImGui::Spacing();
    ImGui::TextUnformatted(lang.misc.font_awesome_credit.c_str());
  }
}

}  // namespace tactile::ui
