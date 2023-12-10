// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/editor_app.hpp"

#include <cstdlib>  // malloc, free

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/core/ui/fonts.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

EditorApp::EditorApp(IWindow* window, IRenderer* renderer)
  : mWindow {require_not_null(window, "null window")},
    mRenderer {require_not_null(renderer, "null renderer")}
{
  // Install Dear ImGui context.
  ImGui::SetCurrentContext(mRenderer->get_imgui_context());
  ImGui::SetAllocatorFunctions([](const usize size, void*) { return std::malloc(size); },
                               [](void* ptr, void*) { std::free(ptr); });

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  auto& style = ImGui::GetStyle();
  ImGui::StyleColorsDark(&style);

}

void EditorApp::on_startup()
{
  TACTILE_LOG_TRACE("Starting up editor...");

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
  try_reload_imgui_fonts(*mRenderer, 13.0f, framebuffer_scale);
}

}  // namespace tactile