// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/engine/engine_app.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/model/settings.hpp"
#include "tactile/core/ui/dialog/new_map_dialog.hpp"
#include "tactile/core/ui/dock/component_dock.hpp"
#include "tactile/core/ui/dock/dock_space.hpp"
#include "tactile/core/ui/dock/document_dock.hpp"
#include "tactile/core/ui/dock/layer_dock.hpp"
#include "tactile/core/ui/dock/log_dock.hpp"
#include "tactile/core/ui/dock/property_dock.hpp"
#include "tactile/core/ui/dock/tileset_dock.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/menu/menu_bar.hpp"

namespace tactile {

class IWindow;
class IRenderer;

/**
 * Represents the Tactile editor application.
 */
class TactileEditor final : public IEngineApp
{
 public:
  TACTILE_DELETE_COPY(TactileEditor);
  TACTILE_DECLARE_MOVE(TactileEditor);

  TactileEditor(IWindow* window, IRenderer* renderer);

  ~TactileEditor() noexcept override;

  void on_startup() override;

  void on_shutdown() override;

  void on_update() override;

  void on_framebuffer_scale_changed(float framebuffer_scale) override;

 private:
  IWindow* mWindow;
  IRenderer* mRenderer;
  Settings mSettings {};
  Maybe<ui::Language> mLanguage {};
  Maybe<Model> mModel {};
  EventDispatcher mEventDispatcher {};
  ui::MenuBar mMenuBar {};
  ui::DockSpace mDockSpace {};
  ui::DocumentDock mDocumentDock {};
  ui::TilesetDock mTilesetDock {};
  ui::LayerDock mLayerDock {};
  ui::PropertyDock mPropertyDock {};
  ui::ComponentDock mComponentDock {};
  ui::LogDock mLogDock {};
  ui::NewMapDialog mNewMapDialog {};
};

}  // namespace tactile
