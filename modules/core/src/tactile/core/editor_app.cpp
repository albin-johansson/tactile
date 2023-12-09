// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/editor_app.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/foundation/debug/validation.hpp"

namespace tactile {

EditorApp::EditorApp(IWindow* window, IRenderer* renderer)
  : mWindow {require_not_null(window, "null window")},
    mRenderer {require_not_null(renderer, "null renderer")}
{}

void EditorApp::on_startup()
{
  TACTILE_LOG_TRACE("Starting up editor...");

  // Install Dear ImGui context.
  const auto imgui_allocator_functions = mRenderer->get_imgui_allocator_functions();
  ImGui::SetCurrentContext(mRenderer->get_imgui_context());
  ImGui::SetAllocatorFunctions(imgui_allocator_functions.alloc_fn,
                               imgui_allocator_functions.free_fn);

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  auto& style = ImGui::GetStyle();
  ImGui::StyleColorsDark(&style);

  mWindow->show();
  mWindow->maximize();
}

void EditorApp::on_shutdown()
{
  TACTILE_LOG_TRACE("Shutting down editor...");

  mWindow->hide();
}

void EditorApp::on_update()
{
  // TODO
}

void EditorApp::on_framebuffer_scale_changed(const float framebuffer_scale)
{
}

}  // namespace tactile