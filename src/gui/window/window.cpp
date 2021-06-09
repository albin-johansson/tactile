#include "window.hpp"

#include "init_ui.hpp"
#include "layer_dock.hpp"
#include "map_document.hpp"
#include "map_editor.hpp"
#include "open_map_dialog.hpp"
#include "preferences.hpp"
#include "properties_dock.hpp"
#include "property_model.hpp"
#include "save_as_dialog.hpp"
#include "setting.hpp"
#include "settings_dialog.hpp"
#include "status_bar.hpp"
#include "tactile_qstring.hpp"
#include "tileset_dock.hpp"
#include "tool_dock.hpp"
#include "ui_window.h"
#include "window_connections.hpp"

namespace tactile::gui {

Window::Window(QWidget* parent)
    : QMainWindow{parent}
    , mUi{init_ui<Ui::window>(this)}
    , mEditor{new map_editor{this}}
    , mToolDock{new tool_dock{this}}
    , mLayerDock{new layer_dock{this}}
    , mTilesetDock{new tileset_dock{this}}
    , mPropertiesDock{new properties_dock{this}}
    , mStatusBar{new status_bar{this}}
    , mToolGroup{new QActionGroup{this}}
{
  setContentsMargins(0, 0, 0, 0);

  setCentralWidget(mEditor);
  addDockWidget(Qt::LeftDockWidgetArea, mPropertiesDock);
  addDockWidget(Qt::RightDockWidgetArea, mTilesetDock);
  addDockWidget(Qt::RightDockWidgetArea, mLayerDock);
  setStatusBar(mStatusBar);

  mToolGroup->setExclusive(true);
  mToolGroup->addAction(mUi->actionStampTool);
  mToolGroup->addAction(mUi->actionBucketTool);
  mToolGroup->addAction(mUi->actionEraserTool);

  window_connections connections;
  connections.init(this);

  showMaximized();

  RestoreLayout();
  EnterNoContentView();
}

Window::~Window() noexcept = default;

void Window::RestoreLayout()
{
  prefs::window::last_layout_geometry().with([this](const QByteArray& geom) {
    restoreGeometry(geom);
  });

  prefs::window::last_layout_state().with([this](const QByteArray& state) {
    restoreState(state);
  });
}

void Window::SaveAs()
{
  save_as_dialog::spawn(
      [this](const QString& path) {
        emit S_SaveAs(path);
      },
      mEditor->active_tab_name().value_or(TACTILE_QSTRING(u"map")));
}

void Window::CenterViewport()
{
  mEditor->center_viewport();
}

void Window::OnResetLayoutAction()
{
  removeDockWidget(mToolDock);
  removeDockWidget(mTilesetDock);
  removeDockWidget(mLayerDock);
  removeDockWidget(mPropertiesDock);

  addDockWidget(Qt::LeftDockWidgetArea, mToolDock);
  addDockWidget(Qt::RightDockWidgetArea, mTilesetDock);
  addDockWidget(Qt::RightDockWidgetArea, mLayerDock);
  addDockWidget(Qt::LeftDockWidgetArea, mPropertiesDock);

  prefs::gfx::reset_tool_widget_visible();
  prefs::gfx::reset_tileset_widget_visible();
  prefs::gfx::reset_layer_widget_visible();
  prefs::gfx::reset_properties_widget_visible();

  mToolDock->setVisible(prefs::gfx::tool_widget_visible().value());
  mTilesetDock->setVisible(prefs::gfx::tileset_widget_visible().value());
  mLayerDock->setVisible(prefs::gfx::layer_widget_visible().value());
  mPropertiesDock->setVisible(prefs::gfx::properties_widget_visible().value());
}

void Window::HideAllDocks()
{
  mToolDock->hide();
  mLayerDock->hide();
  mTilesetDock->hide();
  mPropertiesDock->hide();
}

void Window::RestoreDockVisibility()
{
  prefs::gfx::tool_widget_visible().with([this](const bool value) {
    mToolDock->setVisible(value);
  });

  prefs::gfx::tileset_widget_visible().with([this](const bool value) {
    mTilesetDock->setVisible(value);
  });

  prefs::gfx::layer_widget_visible().with([this](const bool value) {
    mLayerDock->setVisible(value);
  });

  prefs::gfx::properties_widget_visible().with([this](const bool value) {
    mPropertiesDock->setVisible(value);
  });
}

void Window::SetActionsEnabled(const bool enabled)
{
  // File
  mUi->actionCloseMap->setEnabled(enabled);
  mUi->actionSave->setEnabled(enabled);
  mUi->actionSaveAs->setEnabled(enabled);

  // Edit
  mUi->actionAddCol->setEnabled(enabled);
  mUi->actionAddRow->setEnabled(enabled);
  mUi->actionRemoveRow->setEnabled(enabled);
  mUi->actionRemoveCol->setEnabled(enabled);
  mUi->actionResizeMap->setEnabled(enabled);
  mUi->actionStampTool->setEnabled(enabled);
  mUi->actionBucketTool->setEnabled(enabled);
  mUi->actionEraserTool->setEnabled(enabled);
  mUi->actionAddTileset->setEnabled(enabled);

  // View
  mUi->actionResetLayout->setEnabled(enabled);
  mUi->actionToolsVisibility->setEnabled(enabled);
  mUi->actionTilesetsVisibility->setEnabled(enabled);
  mUi->actionLayersVisibility->setEnabled(enabled);
  mUi->actionCenterCamera->setEnabled(enabled);
  mUi->actionToggleGrid->setEnabled(enabled);
  mUi->actionZoomIn->setEnabled(enabled);
  mUi->actionZoomOut->setEnabled(enabled);
  mUi->actionResetZoom->setEnabled(enabled);
  mUi->actionPanUp->setEnabled(enabled);
  mUi->actionPanDown->setEnabled(enabled);
  mUi->actionPanRight->setEnabled(enabled);
  mUi->actionPanLeft->setEnabled(enabled);
}

auto Window::InEditorMode() const -> bool
{
  return mEditor->in_editor_mode();
}

void Window::EnterNoContentView()
{
  mEditor->enable_startup_view();
  mToolDock->disable_tools();

  SetActionsEnabled(false);
  HideAllDocks();

  mStatusBar->set_layer_combo_box_visible(false);
}

void Window::EnterContentView()
{
  mEditor->enable_editor_view();
  mToolDock->enable_tools();

  SetActionsEnabled(true);
  RestoreDockVisibility();

  mStatusBar->set_layer_combo_box_visible(true);
}

void Window::TriggerSaveAs()
{
  SaveAs();
}

void Window::SetActiveTabName(const QString& name)
{
  mEditor->set_active_tab_name(name);
}

void Window::MoveViewport(const int dx, const int dy)
{
  mEditor->move_map(dx, dy);
}

void Window::ForceRedraw()
{
  mEditor->force_redraw();
}

void Window::ShowMapProperties(not_null<core::property_manager*> manager)
{
  mPropertiesDock->show_map_properties(manager);
}

void Window::ShowLayerProperties(not_null<core::property_manager*> manager)
{
  mPropertiesDock->show_layer_properties(manager);
}

void Window::EnableStampPreview(const core::position& position)
{
  mEditor->enable_stamp_preview(position);
}

void Window::DisableStampPreview()
{
  mEditor->disable_stamp_preview();
}

void Window::OnSwitchedMap(const map_id map,
                           not_null<core::map_document*> document)
{
  mTilesetDock->switched_map(map);
  mLayerDock->switched_map(document);
  mPropertiesDock->switched_map(document);
  mStatusBar->switched_map(*document);
}

void Window::OnNewMapAdded(not_null<core::map_document*> document,
                           const map_id id,
                           const QString& name)
{
  Q_ASSERT(document);

  mEditor->add_map_tab(document, id, name);
  mEditor->select_tab(id);
  if (!InEditorMode())
  {
    EnterContentView();
    CenterViewport();
  }

  mUi->actionSave->setDisabled(document->is_clean());
  mTilesetDock->added_map(id, *document);
  mLayerDock->switched_map(document);
  mPropertiesDock->switched_map(document);
  mStatusBar->switched_map(*document);
}

void Window::OnUndoStateUpdated(const bool canUndo)
{
  mUi->actionUndo->setEnabled(canUndo);
}

void Window::OnRedoStateUpdated(const bool canRedo)
{
  mUi->actionRedo->setEnabled(canRedo);
}

void Window::OnUndoTextUpdated(const QString& text)
{
  mUi->actionUndo->setText(TACTILE_QSTRING(u"Undo ") + text);
}

void Window::OnRedoTextUpdated(const QString& text)
{
  mUi->actionRedo->setText(TACTILE_QSTRING(u"Redo ") + text);
}

void Window::OnCleanChanged(const bool clean)
{
  mUi->actionSave->setDisabled(clean);
}

void Window::OnAddedTileset(const map_id map,
                            const tileset_id id,
                            const core::tileset& tileset)
{
  mTilesetDock->added_tileset(map, id, tileset);
}

void Window::OnRemovedTileset(const tileset_id id)
{
  mTilesetDock->removed_tileset(id);
}

void Window::OnRenamedTileset(const tileset_id id, const QString& name)
{
  mTilesetDock->renamed_tileset(id, name);
}

void Window::OnSelectedLayer(const layer_id id, const core::layer& layer)
{
  mStatusBar->set_current_layer(id);
}

void Window::OnAddedLayer(const layer_id id, const core::layer& layer)
{
  mStatusBar->added_layer(id, layer.name());
}

void Window::OnAddedDuplicatedLayer(const layer_id id, const core::layer& layer)
{
  mStatusBar->added_layer(id, layer.name());
}

void Window::OnRemovedLayer(const layer_id id)
{
  mStatusBar->removed_layer(id);
}

void Window::OnAddedProperty(const QString& name)
{
  mPropertiesDock->added_property(name);
}

void Window::OnAboutToRemoveProperty(const QString& name)
{
  mPropertiesDock->about_to_remove_property(name);
}

void Window::OnUpdatedProperty(const QString& name)
{
  mPropertiesDock->updated_property(name);
}

void Window::OnChangedPropertyType(const QString& name)
{
  mPropertiesDock->changed_property_type(name);
}

void Window::OnRenamedProperty(const QString& oldName, const QString& newName)
{
  mPropertiesDock->renamed_property(oldName, newName);
}

void Window::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);
  prefs::window::last_layout_geometry() = saveGeometry();
  prefs::window::last_layout_state() = saveState();
}

void Window::OnThemeChanged()
{
  emit mEditor->theme_changed();
}

void Window::OnReloadOpenGL(const bool enabled)
{
  mEditor->set_opengl_enabled(enabled);
}

void Window::OnStampEnabled()
{
  mUi->actionStampTool->setChecked(true);
  emit S_SelectedTool(tool_id::stamp);
}

void Window::OnBucketEnabled()
{
  mUi->actionBucketTool->setChecked(true);
  emit S_SelectedTool(tool_id::bucket);
}

void Window::OnEraserEnabled()
{
  mUi->actionEraserTool->setChecked(true);
  emit S_SelectedTool(tool_id::eraser);
}

void Window::OnToggleStampAction()
{
  mToolDock->stamp_enabled();
}

void Window::OnToggleBucketAction()
{
  mToolDock->bucket_enabled();
}

void Window::OnToggleEraserAction()
{
  mToolDock->eraser_enabled();
}

void Window::OnAboutToCloseMap(const map_id id)
{
  emit S_AboutToCloseMap(id);
  if (mEditor->tab_count() == 1)
  {
    EnterNoContentView();
  }
}

void Window::OnOpenMapAction()
{
  open_map_dialog::spawn([this](const QString& path) {
    emit S_OpenMap(path);
  });
}

void Window::OnCloseMapAction()
{
  // TODO save current state of open map (exit saves?)
  const auto id = mEditor->active_tab_id().value();

  mEditor->close_tab(id);
  emit S_AboutToCloseMap(id);

  if (mEditor->tab_count() == 0)
  {
    EnterNoContentView();
  }
}

void Window::OnTilesetWidgetVisibilityChanged()
{
  const auto visible = mUi->actionTilesetsVisibility->isChecked();
  mTilesetDock->setVisible(visible);
  prefs::gfx::tileset_widget_visible() = visible;
}

void Window::OnToolWidgetVisibilityChanged()
{
  const auto visible = mUi->actionToolsVisibility->isChecked();
  mToolDock->setVisible(visible);
  prefs::gfx::tool_widget_visible() = visible;
}

void Window::OnLayerWidgetVisibilityChanged()
{
  const auto visible = mUi->actionLayersVisibility->isChecked();
  mLayerDock->setVisible(visible);
  prefs::gfx::layer_widget_visible() = visible;
}

void Window::OnPropertiesWidgetVisibilityChanged()
{
  const auto visible = mUi->actionPropertiesVisibility->isChecked();
  mPropertiesDock->setVisible(visible);
  prefs::gfx::properties_widget_visible() = visible;
}

void Window::OnMouseEntered(QEvent* event)
{
  mStatusBar->set_mouse_info_visible(true);
  emit S_MouseEntered(event);
}

void Window::OnMouseExited(QEvent* event)
{
  mStatusBar->set_mouse_info_visible(false);
  emit S_MouseExited(event);
}

void Window::OnMouseMoved(QMouseEvent* event, QPointF mapPos)
{
  mStatusBar->mouse_moved(event->pos() - mapPos);
  emit S_MouseMoved(event, mapPos);
}

void Window::OnToggleGridAction()
{
  auto grid = prefs::gfx::render_grid();
  grid.with([&, this](bool value) {
    grid = !value;
    ForceRedraw();
  });
}

void Window::OnOpenSettingsAction()
{
  settings_dialog settings{this};

  // clang-format off
  connect(&settings, &settings_dialog::reload_theme,
          this, &Window::OnThemeChanged);

  connect(&settings, &settings_dialog::reload_opengl,
          this, &Window::OnReloadOpenGL);
  // clang-format on

  settings.exec();
}

}  // namespace tactile::gui
