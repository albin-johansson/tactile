// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tactile_editor.hpp"

#include <cstdlib>  // malloc, free
#include <utility>  // move, to_underlying

#include <imgui.h>

#include "tactile/base/int.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/ui/i18n/language_parser.hpp"
#include "tactile/core/ui/shortcuts.hpp"
#include "tactile/render/renderer.hpp"
#include "tactile/render/window.hpp"

namespace tactile {

TACTILE_DEFINE_MOVE(TactileEditor);

TactileEditor::TactileEditor(IWindow* window, IRenderer* renderer)
  : mWindow {require_not_null(window, "null window")},
    mRenderer {require_not_null(renderer, "null renderer")}
{
  // NOLINTBEGIN(*-no-malloc)
  ImGui::SetAllocatorFunctions(
      [](const usize size, void*) { return std::malloc(size); },
      [](void* ptr, void*) { std::free(ptr); });
  ImGui::SetCurrentContext(mRenderer->get_imgui_context());
  // NOLINTEND(*-no-malloc)
}

TactileEditor::~TactileEditor() noexcept = default;

void TactileEditor::on_startup()
{
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  mWindow->show();
  mWindow->maximize();

  mSettings = get_default_settings();

  if (auto language = ui::parse_language_from_disk(mSettings.language)) {
    language->add_icons();
    mLanguage.emplace(std::move(*language));
  }
  else {
    TACTILE_LOG_ERROR("Could not parse language file: {}",
                      language.error().message());
    throw Exception {"could not parse language file"};
  }

  mModel.emplace(&mSettings, &mLanguage.value());
}

void TactileEditor::on_shutdown()
{
  mWindow->hide();
}

void TactileEditor::on_update()
{
  mEventDispatcher.update();

  const auto& model = *mModel;

  ui::push_global_shortcuts(model, mEventDispatcher);
}

void TactileEditor::on_framebuffer_scale_changed(const float framebuffer_scale)
{}

}  // namespace tactile
