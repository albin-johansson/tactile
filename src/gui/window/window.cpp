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

namespace tactile {

Window::Window(QWidget* parent)
    : QMainWindow{parent}
    , mUi{InitUi<Ui::Window>(this)}
    , mEditor{new MapEditor{this}}
    , mToolDock{new ToolDock{this}}
    , mLayerDock{new LayerDock{this}}
    , mTilesetDock{new TilesetDock{this}}
    , mPropertiesDock{new PropertiesDock{this}}
    , mStatusBar{new StatusBar{this}}
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

  WindowConnections connections;
  connections.Init(this);

  showMaximized();

  RestoreLayout();
  EnterNoContentView();
}

Window::~Window() noexcept = default;

void Window::RestoreLayout()
{
  prefs::window::last_layout_geometry().with(
      [this](const QByteArray& geom) { restoreGeometry(geom); });

  prefs::window::last_layout_state().with(
      [this](const QByteArray& state) { restoreState(state); });
}

void Window::SaveAs()
{
  SaveAsDialog::Spawn(
      [this](const QString& path) { emit S_SaveAs(path); },
      mEditor->ActiveTabName().value_or(TACTILE_QSTRING(u"map")));
}

void Window::CenterViewport()
{
  mEditor->CenterViewport();
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
  prefs::gfx::tool_widget_visible().with(
      [this](const bool value) { mToolDock->setVisible(value); });

  prefs::gfx::tileset_widget_visible().with(
      [this](const bool value) { mTilesetDock->setVisible(value); });

  prefs::gfx::layer_widget_visible().with(
      [this](const bool value) { mLayerDock->setVisible(value); });

  prefs::gfx::properties_widget_visible().with(
      [this](const bool value) { mPropertiesDock->setVisible(value); });
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
  return mEditor->InEditorMode();
}

void Window::EnterNoContentView()
{
  mEditor->EnableStartupView();
  mToolDock->DisableTools();

  SetActionsEnabled(false);
  HideAllDocks();

  mStatusBar->SetLayerComboBoxVisible(false);
}

void Window::EnterContentView()
{
  mEditor->EnableEditorView();
  mToolDock->EnableTools();

  SetActionsEnabled(true);
  RestoreDockVisibility();

  mStatusBar->SetLayerComboBoxVisible(true);
}

void Window::TriggerSaveAs()
{
  SaveAs();
}

void Window::SetActiveTabName(const QString& name)
{
  mEditor->SetActiveTabName(name);
}

void Window::MoveViewport(const int dx, const int dy)
{
  mEditor->MoveViewport(dx, dy);
}

void Window::ForceRedraw()
{
  mEditor->ForceRedraw();
}

void Window::ShowMapProperties(not_null<core::property_manager*> manager)
{
  mPropertiesDock->ShowMapProperties(manager);
}

void Window::ShowLayerProperties(not_null<core::property_manager*> manager)
{
  mPropertiesDock->ShowLayerProperties(manager);
}

void Window::EnableStampPreview(const core::position& position)
{
  mEditor->EnableStampPreview(position);
}

void Window::DisableStampPreview()
{
  mEditor->DisableStampPreview();
}

void Window::OnSwitchedMap(const map_id map,
                           not_null<core::map_document*> document)
{
  mTilesetDock->OnSwitchedMap(map);
  mLayerDock->OnSwitchedMap(document);
  mPropertiesDock->OnSwitchedMap(document);
  mStatusBar->OnSwitchedMap(*document);
}

void Window::OnNewMapAdded(not_null<core::map_document*> document,
                           const map_id id,
                           const QString& name)
{
  Q_ASSERT(document);

  mEditor->AddMapTab(document, id, name);
  mEditor->SelectTab(id);
  if (!InEditorMode())
  {
    EnterContentView();
    CenterViewport();
  }

  mUi->actionSave->setDisabled(document->is_clean());
  mTilesetDock->OnAddedMap(id, *document);
  mLayerDock->OnSwitchedMap(document);
  mPropertiesDock->OnSwitchedMap(document);
  mStatusBar->OnSwitchedMap(*document);
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
  mTilesetDock->OnAddedTileset(map, id, tileset);
}

void Window::OnRemovedTileset(const tileset_id id)
{
  mTilesetDock->OnRemovedTileset(id);
}

void Window::OnRenamedTileset(const tileset_id id, const QString& name)
{
  mTilesetDock->OnRenamedTileset(id, name);
}

void Window::OnSelectedLayer(const layer_id id, const core::layer& layer)
{
  mStatusBar->SetCurrentLayer(id);
}

void Window::OnAddedLayer(const layer_id id, const core::layer& layer)
{
  mStatusBar->OnAddedLayer(id, layer.name());
}

void Window::OnAddedDuplicatedLayer(const layer_id id, const core::layer& layer)
{
  mStatusBar->OnAddedLayer(id, layer.name());
}

void Window::OnRemovedLayer(const layer_id id)
{
  mStatusBar->OnRemovedLayer(id);
}

void Window::OnAddedProperty(const QString& name)
{
  mPropertiesDock->OnAddedProperty(name);
}

void Window::OnAboutToRemoveProperty(const QString& name)
{
  mPropertiesDock->OnAboutToRemoveProperty(name);
}

void Window::OnUpdatedProperty(const QString& name)
{
  mPropertiesDock->OnUpdatedProperty(name);
}

void Window::OnChangedPropertyType(const QString& name)
{
  mPropertiesDock->OnChangedPropertyType(name);
}

void Window::OnRenamedProperty(const QString& oldName, const QString& newName)
{
  mPropertiesDock->OnRenamedProperty(oldName, newName);
}

void Window::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);
  prefs::window::last_layout_geometry() = saveGeometry();
  prefs::window::last_layout_state() = saveState();
}

void Window::OnThemeChanged()
{
  emit mEditor->S_ThemeChanged();
}

void Window::OnReloadOpenGL(const bool enabled)
{
  mEditor->SetOpenGlEnabled(enabled);
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
  mToolDock->OnEnableStamp();
}

void Window::OnToggleBucketAction()
{
  mToolDock->OnEnableBucket();
}

void Window::OnToggleEraserAction()
{
  mToolDock->OnEnableEraser();
}

void Window::OnAboutToCloseMap(const map_id id)
{
  emit S_AboutToCloseMap(id);
  if (mEditor->TabCount() == 1)
  {
    EnterNoContentView();
  }
}

void Window::OnOpenMapAction()
{
  OpenMapDialog::Spawn([this](const QString& path) { emit S_OpenMap(path); });
}

void Window::OnCloseMapAction()
{
  // TODO save current state of open map (exit saves?)
  const auto id = mEditor->ActiveTabID().value();

  mEditor->CloseTab(id);
  emit S_AboutToCloseMap(id);

  if (mEditor->TabCount() == 0)
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
  mStatusBar->SetMouseInfoVisible(true);
  emit S_MouseEntered(event);
}

void Window::OnMouseExited(QEvent* event)
{
  mStatusBar->SetMouseInfoVisible(false);
  emit S_MouseExited(event);
}

void Window::OnMouseMoved(QMouseEvent* event, QPointF mapPos)
{
  mStatusBar->OnMouseMoved(event->pos() - mapPos);
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
  SettingsDialog settings{this};

  // clang-format off
  connect(&settings, &SettingsDialog::S_ReloadTheme,
          this, &Window::OnThemeChanged);

  connect(&settings, &SettingsDialog::S_ReloadOpenGl,
          this, &Window::OnReloadOpenGL);
  // clang-format on

  settings.exec();
}

}  // namespace tactile
