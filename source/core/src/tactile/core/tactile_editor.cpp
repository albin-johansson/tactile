// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tactile_editor.hpp"

#include <utility>  // move

#include "tactile/base/render/renderer.hpp"
#include "tactile/base/render/window.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/event/view_events.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/ui/i18n/language_parser.hpp"

namespace tactile {

TactileEditor::TactileEditor(IWindow* window, IRenderer* renderer)
  : mWindow {require_not_null(window, "null window")},
    mRenderer {require_not_null(renderer, "null renderer")}
{}

TactileEditor::~TactileEditor() noexcept = default;

void TactileEditor::on_startup()
{
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

  auto& model = mModel.emplace(&mSettings, &mLanguage.value());

  auto& file_event_handler = mFileEventHandler.emplace(&model);
  auto& edit_event_handler = mEditEventHandler.emplace(&model);
  auto& view_event_handler =
      mViewEventHandler.emplace(&model, mRenderer, &mWidgetManager);
  auto& tileset_event_handler =
      mTilesetEventHandler.emplace(&model, mRenderer, &mWidgetManager);
  auto& map_event_handler = mMapEventHandler.emplace(&model, &mWidgetManager);
  auto& layer_event_handler = mLayerEventHandler.emplace(&model);
  auto& component_event_handler =
      mComponentEventHandler.emplace(&model, &mWidgetManager);
  auto& property_event_handler =
      mPropertyEventHandler.emplace(&model, &mWidgetManager);
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

void TactileEditor::on_shutdown()
{
  mWindow->hide();
}

void TactileEditor::on_update()
{
  mEventDispatcher.update();
}

void TactileEditor::on_render()
{
  mWidgetManager.push(*mModel, mEventDispatcher);
}

void TactileEditor::on_framebuffer_scale_changed(const float framebuffer_scale)
{
  mEventDispatcher.push<ReloadFontsEvent>(framebuffer_scale);
}

}  // namespace tactile
