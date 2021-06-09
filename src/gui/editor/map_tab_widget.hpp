#pragma once

#include <QString>  // QString

#include "forward_declare.hpp"
#include "map_view.hpp"
#include "maybe.hpp"
#include "tab_widget.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::gui {

/**
 * \class map_tab_widget
 *
 * \brief Represents the tab widget that holds all of the map views.
 *
 * \headerfile map_tab_widget.hpp
 */
class map_tab_widget final : public TabWidget
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
  explicit map_tab_widget(QWidget* parent = nullptr);

  ~map_tab_widget() noexcept override;

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
  void add_map_tab(core::map_document* map, map_id id, const QString& title);

  /**
   * \brief Removes the tab associated with the specified map.
   *
   * \param id the identifier associated with the map that will be removed.
   *
   * \since 0.1.0
   */
  void remove_map_tab(map_id id);

  /**
   * \brief Selects the tab associated with the specified map.
   *
   * \param id the identifier associated with the map that will be selected.
   *
   * \since 0.1.0
   */
  void select_tab(map_id id);

  /**
   * \brief Centers the currently active map to fit the viewport.
   *
   * \since 0.1.0
   */
  void center_map();

  /**
   * \brief Moves the currently active map.
   *
   * \param dx the amount to move the map by along the x-axis.
   * \param dy the amount to move the map by along the y-axis.
   *
   * \since 0.1.0
   */
  void move_map(int dx, int dy);

  void enable_stamp_preview(const core::position& position);

  void disable_stamp_preview();

  void show_properties();

  /**
   * \brief Sets the name of the currently active tab.
   *
   * \details This function has no effect if there is no active tab.
   *
   * \param name the new name of the tab.
   *
   * \since 0.1.0
   */
  void set_active_tab_name(const QString& name);

  void set_opengl_enabled(bool enabled);

  /**
   * \brief Returns the map ID associated with the currently active tab.
   *
   * \return the map ID of the active tab; `std::nullopt` if there is no active
   * tab.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto active_tab_id() const -> maybe<map_id>;

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
  [[nodiscard]] auto id_from_index(int index) const -> maybe<map_id>;

  [[nodiscard]] auto active_tab_name() const -> maybe<QString>;

 signals:
  void ui_remove_map(map_id id);
  void increase_zoom();
  void decrease_zoom();
  void mouse_pressed(QMouseEvent* event, QPointF mapPosition);
  void mouse_moved(QMouseEvent* event, QPointF mapPosition);
  void mouse_released(QMouseEvent* event, QPointF mapPosition);
  void mouse_entered(QEvent* event);
  void mouse_exited(QEvent* event);
  void spawn_context_menu(const QPoint& pos);

 public slots:
  void theme_changed();

  void force_redraw();

 private:
  [[nodiscard]] auto current_view() -> map_view*;

  [[nodiscard]] auto get_view(int index) -> map_view*;

  [[nodiscard]] auto get_view(int index) const -> const map_view*;

  [[nodiscard]] auto view_from_id(map_id id) -> map_view*;

 private slots:
  void handle_tab_close(int index);
};

}  // namespace tactile::gui
