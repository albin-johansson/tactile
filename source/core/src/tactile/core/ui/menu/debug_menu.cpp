// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/menu/debug_menu.hpp"

#include <tuple>  // ignore

#include <imgui.h>

#include "tactile/core/model/model.hpp"
#include "tactile/core/platform/filesystem.hpp"
#include "tactile/core/ui/common/menus.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {

void DebugMenu::push(const Model& model)
{
  const auto& language = model.get_language();

  const MenuScope menu {language.get(StringID::kDebugMenu)};
  if (menu.is_open()) {
    if (ImGui::MenuItem(language.get(StringID::kOpenDebugger),
                        nullptr,
                        mShowDebugger)) {
      mShowDebugger = !mShowDebugger;
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kOpenStyleEditor),
                        nullptr,
                        mShowStyle)) {
      mShowStyle = !mShowStyle;
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kOpenDemoWindow),
                        nullptr,
                        mShowDemo)) {
      mShowDemo = !mShowDemo;
    }

    ImGui::Separator();

    if (ImGui::MenuItem(language.get(StringID::kOpenStorageDir))) {
      if (const auto persistent_dir = get_persistent_storage_directory()) {
        std::ignore = open_directory_in_finder(*persistent_dir);
      }
    }
  }

  if (mShowDebugger) {
    ImGui::ShowMetricsWindow(&mShowDebugger);
  }

  if (mShowDemo) {
    ImGui::ShowDemoWindow(&mShowDemo);
  }

  if (mShowStyle) {
    const Window style_editor {language.get(StringID::kStyleEditorWidget),
                               ImGuiWindowFlags_None,
                               &mShowStyle};
    if (style_editor.is_open()) {
      ImGui::ShowStyleEditor();
    }
  }
}

}  // namespace tactile::ui
