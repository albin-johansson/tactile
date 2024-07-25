// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/dock/dock_space.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/platform/filesystem.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {

DockSpace::DockSpace()
  : mImGuiIniPath {get_imgui_ini_file_path()}
{}

void DockSpace::update(const Language& language)
{
  const auto* viewport =
      require_not_null(ImGui::GetMainViewport(), "null viewport");
  const auto flags = ImGuiDockNodeFlags_NoDockingOverCentralNode;

  mRootId = ImGui::DockSpaceOverViewport(0, viewport, flags);

  if (!mDidInit && viewport->Size.x > 0 && viewport->Size.y > 0) {
    if (!fs::exists(mImGuiIniPath)) {
      reset_layout(language);
    }

    mDidInit = true;
  }
}

void DockSpace::reset_layout(const Language& language)
{
  const SetLogScope log_scope {"DockSpace"};
  TACTILE_LOG_DEBUG("Resetting layout");

  const auto root_node = mRootId.value();
  ImGui::DockBuilderRemoveNodeChildNodes(root_node);

  ImGuiID viewport_node {};
  ImGuiID right_node {};
  ImGui::DockBuilderSplitNode(root_node,
                              ImGuiDir_Right,
                              0.33f,
                              &right_node,
                              &viewport_node);

  ImGuiID right_top_node {};
  ImGuiID right_bottom_node {};
  ImGui::DockBuilderSplitNode(right_node,
                              ImGuiDir_Down,
                              0.5f,
                              &right_bottom_node,
                              &right_top_node);

  ImGuiID bottom_node {};
  ImGui::DockBuilderSplitNode(viewport_node,
                              ImGuiDir_Down,
                              0.25f,
                              &bottom_node,
                              nullptr);

  ImGui::DockBuilderDockWindow(language.get(StringID::kDocumentDock),
                               viewport_node);
  ImGui::DockBuilderDockWindow(language.get(StringID::kTilesetDock),
                               right_top_node);
  ImGui::DockBuilderDockWindow(language.get(StringID::kPropertyDock),
                               right_top_node);
  ImGui::DockBuilderDockWindow(language.get(StringID::kComponentDock),
                               right_top_node);
  ImGui::DockBuilderDockWindow(language.get(StringID::kLayerDock),
                               right_bottom_node);
  ImGui::DockBuilderDockWindow(language.get(StringID::kLogDock), bottom_node);

  ImGui::DockBuilderFinish(root_node);
}

}  // namespace tactile::ui
