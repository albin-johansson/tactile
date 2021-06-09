#pragma once

#include <QMouseEvent>  // QMouseEvent
#include <QWidget>      // QWidget

#include "forward_declare.hpp"
#include "map_id.hpp"
#include "maybe.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE_UI(MapEditor)
TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile, MapTabWidget)
TACTILE_FORWARD_DECLARE(tactile, MapEditorContextMenu)

namespace tactile {

/**
 * \class MapEditor
 *
 * \brief Represents the center stage of the editor and contains the main
 * map editor pane.
 *
 * \since 0.1.0
 *
 * \headerfile map_editor.hpp
 */
class MapEditor final : public QWidget
{
  Q_OBJECT

 public:
  explicit MapEditor(QWidget* parent = nullptr);

  ~MapEditor() noexcept override;

  void EnableStampPreview(const core::position& position);

  void DisableStampPreview();

  void AddMapTab(core::map_document* map, map_id id, const QString& title);

  void SelectTab(map_id id);

  void CloseTab(map_id id) noexcept;

  void CenterViewport();

  void MoveViewport(int dx, int dy) noexcept;

  /**
   * \brief Enables the startup view.
   *
   * \details The startup view is the main page show upon startup or when no
   * maps are active.
   *
   * \since 0.1.0
   */
  void EnableStartupView() noexcept;

  /**
   * \brief Enables the main editor view.
   *
   * \since 0.1.0
   */
  void EnableEditorView() noexcept;

  /**
   * \copydoc map_tab_widget::SetActiveTabName()
   */
  void SetActiveTabName(const QString& name);

  [[nodiscard]] auto InEditorMode() const -> bool;

  [[nodiscard]] auto ActiveTabID() const -> maybe<map_id>;

  [[nodiscard]] auto ActiveTabName() const -> maybe<QString>;

  [[nodiscard]] auto TabCount() const -> int;

 public slots:
  void ForceRedraw();
  void SetOpenGlEnabled(bool enabled);

 signals:
  void S_RemoveMap(map_id id);
  void S_SelectMap(map_id id);
  void S_ZoomIn();
  void S_ZoomOut();
  void S_ThemeChanged();
  void S_MousePressed(QMouseEvent* event, QPointF mapPosition);
  void S_MouseMoved(QMouseEvent* event, QPointF mapPosition);
  void S_MouseReleased(QMouseEvent* event, QPointF mapPosition);
  void S_MouseEntered(QEvent* event);
  void S_MouseExited(QEvent* event);

 private:
  unique<Ui::MapEditor> mUi;
  MapTabWidget* mTabWidget{};
  MapEditorContextMenu* mContextMenu{};
  int mStartupID{};
  int mEditorID{};

  void InitConnections();

 private slots:
  void OnTabChanged(int index);
  void OnSpawnContextMenu(const QPoint& pos);
  void OnShowMapProperties();
};

}  // namespace tactile
