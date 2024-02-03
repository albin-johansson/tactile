// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/editor_app.hpp"

#include <cstdlib>  // malloc, free

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/core/ui/common/styling.hpp"
#include "tactile/core/ui/fonts.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

EditorApp::EditorApp(IWindow* window, IRenderer* renderer)
  : mWindow {require_not_null(window, "null window")},
    mRenderer {require_not_null(renderer, "null renderer")},
    mModel {make_unique<Model>()}
{
  // Install Dear ImGui context.
  ImGui::SetAllocatorFunctions(
      [](const usize size, void*) { return std::malloc(size); },  // NOLINT(*-no-malloc)
      [](void* ptr, void*) { std::free(ptr); });                  // NOLINT(*-no-malloc)
  ImGui::SetCurrentContext(mRenderer->get_imgui_context());

  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  auto& style = ImGui::GetStyle();
  ImGui::StyleColorsDark(&style);
  apply_custom_style(style);

  load_languages("assets/lang")
      .transform([this](LanguageMap&& languages) {
        mModel->set_languages(std::move(languages));
      })
      .transform_error([](const ErrorCode& error_code) {
        TACTILE_LOG_ERROR("Could not load translation files: {}", error_code.message());
        throw Exception {"Could not load translation files"};
      });
}

EditorApp::~EditorApp() noexcept = default;

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

  ImGui::SetCurrentContext(nullptr);
}

void EditorApp::on_update()
{
  // TODO
}

void EditorApp::on_framebuffer_scale_changed(const float framebuffer_scale)
{
  try_reload_imgui_fonts(*mRenderer, 13.0f, framebuffer_scale);
}

}  // namespace tactile::core