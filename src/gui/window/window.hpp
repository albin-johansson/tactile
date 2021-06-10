#pragma once

#include <QActionGroup>  // QActionGroup
#include <QMainWindow>   // QMainWindow
#include <QString>       // QString
#include <QWidget>       // QWidget

#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "map_id.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "property.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"
#include "tool_id.hpp"

TACTILE_FORWARD_DECLARE_UI(Window)

TACTILE_FORWARD_DECLARE(tactile::core, ILayer)
TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::core, property_manager)
TACTILE_FORWARD_DECLARE(tactile, MapEditor)
TACTILE_FORWARD_DECLARE(tactile, ToolDock)
TACTILE_FORWARD_DECLARE(tactile, LayerDock)
TACTILE_FORWARD_DECLARE(tactile, TilesetDock)
TACTILE_FORWARD_DECLARE(tactile, PropertiesDock)
TACTILE_FORWARD_DECLARE(tactile, StatusBar)

namespace tactile {

/**
 * \class Window
 *
 * \brief Represents the main window used in the application.
 *
 * \see `QMainWindow`
 *
 * \since 0.1.0
 *
 * \headerfile window.hpp
 */
class Window final : public QMainWindow
{
  Q_OBJECT

  friend class WindowConnections;

 public:
  /**
   * \param parent a pointer to the parent widget, defaults to null.
   *
   * \since 0.1.0
   */
  explicit Window(QWidget* parent = nullptr);

  ~Window() noexcept override;

  /// Enables the view that is shown when no maps are active.
  void EnterNoContentView();

  /// Enables the main editor view.
  void EnterContentView();

  void TriggerSaveAs();

  void SetActiveTabName(const QString& name);

  void MoveViewport(int dx, int dy);

 signals:
  void S_Save();
  void S_SaveAs(const QString& path);
  void S_OpenMap(const QString& path);

  void S_NewMap();
  void S_AboutToCloseMap(map_id id);

  void S_Undo();
  void S_Redo();

  void S_AddRow();
  void S_AddCol();

  void S_RemoveRow();
  void S_RemoveCol();

  void S_SelectedTool(tool_id tool);

  void S_AddTileset();
  void S_RemoveTileset(tileset_id id);
  void S_SelectTileset(tileset_id id);
  void S_RenameTileset(tileset_id id, const QString& name);
  void S_SetTilesetSelection(const core::tileset_selection& selection);

  void S_ResizeMap();

  void S_ZoomIn();
  void S_ZoomOut();
  void S_ResetZoom();

  void S_PanRight();
  void S_PanDown();
  void S_PanLeft();
  void S_PanUp();

  void S_SelectMap(map_id id);

  void S_MousePressed(QMouseEvent* event, QPointF mapPosition);
  void S_MouseMoved(QMouseEvent* event, QPointF mapPosition);
  void S_MouseReleased(QMouseEvent* event, QPointF mapPosition);
  void S_MouseEntered(QEvent* event);
  void S_MouseExited(QEvent* event);

 public slots:
  void ForceRedraw();

  void ShowMapProperties(not_null<core::property_manager*> manager);
  void ShowLayerProperties(not_null<core::property_manager*> manager);

  void EnableStampPreview(const core::position& position);
  void DisableStampPreview();

  void OnSwitchedMap(map_id map, not_null<core::map_document*> document);

  void OnNewMapAdded(not_null<core::map_document*> document,
                     map_id id,
                     const QString& name = TACTILE_QSTRING(u"map"));

  /// \name Undo/Redo slots
  /// \{

  void OnUndoStateUpdated(bool canUndo);
  void OnRedoStateUpdated(bool canRedo);
  void OnUndoTextUpdated(const QString& text);
  void OnRedoTextUpdated(const QString& text);
  void OnCleanChanged(bool clean);

  /// \} End of undo/redo slots

  /// \name Tileset slots
  /// \{

  void OnAddedTileset(map_id map, tileset_id id, const core::tileset& tileset);
  void OnRemovedTileset(tileset_id id);
  void OnRenamedTileset(tileset_id id, const QString& name);

  /// \} End of tileset slots

  /// \name Layer slots
  /// \{

  void OnSelectedLayer(layer_id id, const core::ILayer& layer);
  void OnAddedLayer(layer_id id, const core::ILayer& layer);
  void OnAddedDuplicatedLayer(layer_id id, const core::ILayer& layer);
  void OnRemovedLayer(layer_id id);

  /// \} End of layer slots

  /// \name Property slots
  /// \{

  void OnAddedProperty(const QString& name);
  void OnAboutToRemoveProperty(const QString& name);
  void OnUpdatedProperty(const QString& name);
  void OnChangedPropertyType(const QString& name);
  void OnRenamedProperty(const QString& oldName, const QString& newName);

  /// \} End of property slots

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  unique<Ui::Window> mUi;
  MapEditor* mEditor{};
  ToolDock* mToolDock{};
  LayerDock* mLayerDock{};
  TilesetDock* mTilesetDock{};
  PropertiesDock* mPropertiesDock{};
  StatusBar* mStatusBar{};
  QActionGroup* mToolGroup{};

  void RestoreLayout();

  void HideAllDocks();

  void RestoreDockVisibility();

  void SetActionsEnabled(bool enabled);

  /**
   * \brief Indicates whether or not the editor view is enabled.
   *
   * \return `true` if the editor view is enabled; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto InEditorMode() const -> bool;

 private slots:
  void SaveAs();

  void CenterViewport();

  void OnResetLayoutAction();

  void OnThemeChanged();

  void OnReloadOpenGL(bool enabled);

  void OnStampEnabled();
  void OnBucketEnabled();
  void OnEraserEnabled();

  void OnToggleStampAction();
  void OnToggleBucketAction();
  void OnToggleEraserAction();

  void OnAboutToCloseMap(map_id id);
  void OnOpenMapAction();
  void OnCloseMapAction();

  void OnTilesetWidgetVisibilityChanged();
  void OnToolWidgetVisibilityChanged();
  void OnLayerWidgetVisibilityChanged();
  void OnPropertiesWidgetVisibilityChanged();

  void OnMouseEntered(QEvent* event);
  void OnMouseExited(QEvent* event);
  void OnMouseMoved(QMouseEvent* event, QPointF mapPos);

  void OnToggleGridAction();

  void OnOpenSettingsAction();
};

}  // namespace tactile
