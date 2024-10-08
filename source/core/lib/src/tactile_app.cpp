// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tactile_app.hpp"

#include <utility>  // move

#include <imgui.h>

#include "tactile/base/render/renderer.hpp"
#include "tactile/base/render/window.hpp"
#include "tactile/base/runtime.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/event/view_events.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/ui/common/style.hpp"
#include "tactile/core/ui/i18n/language_parser.hpp"

namespace tactile {

TactileApp::TactileApp(IRuntime* runtime)
  : mRuntime {require_not_null(runtime, "null runtime")},
    mWindow {require_not_null(runtime->get_window(), "null window")},
    mRenderer {require_not_null(runtime->get_renderer(), "null renderer")}
{}

TactileApp::~TactileApp() noexcept = default;

void TactileApp::on_startup()
{
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ui::apply_custom_style(ImGui::GetStyle());

  mWindow->show();
  mWindow->maximize();

  mSettings = get_default_settings();

  if (auto language = ui::parse_language_from_disk(mSettings.language)) {
    language->add_icons();
    mLanguage.emplace(std::move(*language));
  }
  else {
    TACTILE_LOG_ERROR("Could not parse language file: {}", to_string(language.error()));
    throw Exception {"could not parse language file"};
  }

  auto& model = mModel.emplace(&mSettings, &mLanguage.value());

  auto& file_event_handler = mFileEventHandler.emplace(&model, mRuntime);
  auto& edit_event_handler = mEditEventHandler.emplace(&model);
  auto& view_event_handler = mViewEventHandler.emplace(&model, mRenderer, &mWidgetManager);
  auto& tileset_event_handler =
      mTilesetEventHandler.emplace(&model, mRenderer, &mWidgetManager);
  auto& map_event_handler = mMapEventHandler.emplace(&model, &mWidgetManager, mRuntime);
  auto& layer_event_handler = mLayerEventHandler.emplace(&model);
  auto& component_event_handler = mComponentEventHandler.emplace(&model, &mWidgetManager);
  auto& property_event_handler = mPropertyEventHandler.emplace(&model, &mWidgetManager);
  auto& viewport_event_handler = mViewportEventHandler.emplace(&model);

  file_event_handler.install(mEventDispatcher);
  edit_event_handler.install(mEventDispatcher);
  view_event_handler.install(mEventDispatcher);
  tileset_event_handler.install(mEventDispatcher);
  map_event_handler.install(mEventDispatcher);
  layer_event_handler.install(mEventDispatcher);
  component_event_handler.install(mEventDispatcher);
  property_event_handler.install(mEventDispatcher);
  viewport_event_handler.install(mEventDispatcher);
}

void TactileApp::on_shutdown()
{
  mWindow->hide();
}

void TactileApp::on_update()
{
  mEventDispatcher.update();
}

void TactileApp::on_render()
{
  mWidgetManager.push(*mModel, mEventDispatcher);
}

void TactileApp::on_framebuffer_scale_changed(const float framebuffer_scale)
{
  mEventDispatcher.push<ReloadFontsEvent>(framebuffer_scale);
}

}  // namespace tactile
