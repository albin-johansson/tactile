#pragma once

#include <QString>  // QString

#include "forward_declare.hpp"
#include "map_view.hpp"
#include "maybe.hpp"
#include "tab_widget.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile {

/**
 * \class MapTabWidget
 *
 * \brief Represents the tab widget that holds all of the map views.
 *
 * \headerfile map_tab_widget.hpp
 */
class MapTabWidget final : public TabWidget
{
  Q_OBJECT

 public:
  /**
   * \brief Creates a map tab widget.
   *
   * \param parent a pointer to the parent widget.
   *
   * \since 0.1.0
   */
  explicit MapTabWidget(QWidget* parent = nullptr);

  ~MapTabWidget() noexcept override;

  /**
   * \brief Adds a map tab.
   *
   * \param map a pointer to the map document that will be associated with the
   * tab.
   * \param id the identifier associated with the map.
   * \param title the title that will be visible in the tab.
   *
   * \since 0.1.0
   */
  void AddTab(core::MapDocument* map, map_id id, const QString& title);

  /**
   * \brief Removes the tab associated with the specified map.
   *
   * \param id the identifier associated with the map that will be removed.
   *
   * \since 0.1.0
   */
  void RemoveTab(map_id id);

  /**
   * \brief Selects the tab associated with the specified map.
   *
   * \param id the identifier associated with the map that will be selected.
   *
   * \since 0.1.0
   */
  void SelectTab(map_id id);

  /**
   * \brief Centers the currently active map to fit the viewport.
   *
   * \since 0.1.0
   */
  void CenterViewport();

  /**
   * \brief Moves the currently active map.
   *
   * \param dx the amount to move the map by along the x-axis.
   * \param dy the amount to move the map by along the y-axis.
   *
   * \since 0.1.0
   */
  void MoveViewport(int dx, int dy);

  void EnableStampPreview(const core::position& position);

  void DisableStampPreview();

  void ShowMapProperties();

  /**
   * \brief Sets the name of the currently active tab.
   *
   * \details This function has no effect if there is no active tab.
   *
   * \param name the new name of the tab.
   *
   * \since 0.1.0
   */
  void SetActiveTabName(const QString& name);

  void SetOpenGlEnabled(bool enabled);

  /**
   * \brief Returns the map ID associated with the currently active tab.
   *
   * \return the map ID of the active tab; `std::nullopt` if there is no active
   * tab.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto ActiveTabId() const -> maybe<map_id>;

  /**
   * \brief Returns the map ID associated with the specified tab.
   *
   * \param index the index of the tab that will be queried.
   *
   * \return the map ID of the specified tab; `std::nullopt` if no such ID
   * exists.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto IdFromIndex(int index) const -> maybe<map_id>;

  [[nodiscard]] auto ActiveTabName() const -> maybe<QString>;

 signals:
  void S_RemoveMap(map_id id);
  void S_ZoomIn();
  void S_ZoomOut();
  void S_MousePressed(QMouseEvent* event, QPointF mapPosition);
  void S_MouseMoved(QMouseEvent* event, QPointF mapPosition);
  void S_MouseReleased(QMouseEvent* event, QPointF mapPosition);
  void S_MouseEntered(QEvent* event);
  void S_MouseExited(QEvent* event);
  void S_SpawnContextMenu(const QPoint& pos);

 public slots:
  void ForceRedraw();

  void OnThemeChanged();

 private:
  [[nodiscard]] auto CurrentView() -> MapView*;

  [[nodiscard]] auto GetView(int index) -> MapView*;

  [[nodiscard]] auto GetView(int index) const -> const MapView*;

  [[nodiscard]] auto ViewFromId(map_id id) -> MapView*;

 private slots:
  void OnTabCloseRequested(int index);
};

}  // namespace tactile
