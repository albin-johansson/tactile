#pragma once

#include <qwidget.h>

#include <map>
#include <optional>  // optional
#include <vector>    // vector, erase_if

#include "fwd.hpp"
#include "position.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace Ui {
class tileset_content_page;
}

namespace tactile::gui {

/**
 * @class tileset_content_page
 *
 * @brief Represents the contents of a tileset widget when there are at least
 * one available tileset.
 *
 * @since 0.1.0
 *
 * @headerfile tileset_content_page.hpp
 */
class tileset_content_page final : public QWidget
{
  Q_OBJECT

  using tileset_map = std::map<tileset_id, tileset_tab*>;
  using tilemap_map = std::map<map_id, tileset_map>;
  using const_iterator = tilemap_map::const_iterator;

 public:
  /**
   * @brief Creates a `tileset_content_page` instance.
   *
   * @param parent a pointer to the parent widget.
   *
   * @since 0.1.0
   */
  explicit tileset_content_page(QWidget* parent = nullptr);

  ~tileset_content_page() noexcept override;

  /**
   * @brief Adds a tileset tab that represents the supplied tileset.
   *
   * @pre `map` must not have been added before.
   * @pre `id` must not have been added before.
   *
   * @param map the ID associated with the map that owns the tileset.
   * @param id the ID associated with the tileset that will be added.
   * @param tileset the tileset that will be added.
   *
   * @since 0.1.0
   */
  void add_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  /**
   * @brief Removes the tileset tab associated with the specified tileset.
   *
   * @pre `id` must be associated with an existing tileset tab.
   *
   * @param id the ID associated with the tileset tab which will be removed.
   *
   * @since 0.1.0
   */
  void remove_tileset(tileset_id id);

  /**
   * @brief Indicates whether or not there are any tileset tabs.
   *
   * @return `true` if there are no tileset tabs; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto empty() const -> bool;

 signals:
  /**
   * @brief Requests the tileset widget to switch to the empty page, since no
   * tilesets are available.
   *
   * @since 0.1.0
   */
  void switch_to_empty_page();

  void switch_to_content_page();

  /**
   * @brief Requests a new tileset to be added to the model.
   *
   * @since 0.1.0
   */
  void add_new_tileset();

  /**
   * @brief Dispatched when the active tileset has changed.
   *
   * @param id the ID of the selected tileset.
   *
   * @since 0.1.0
   */
  void selected_tileset(tileset_id id);

  /**
   * @brief Dispatched when a tileset has been removed.
   *
   * @param id the ID of the tileset that was removed.
   *
   * @since 0.1.0
   */
  void removed_tileset(tileset_id id);

  /**
   * @brief Dispatched when the tile selection has changed in the current
   * tileset.
   *
   * @param topLeft the position of the top-left tile.
   * @param bottomRight the position of the bottom-right tile.
   *
   * @since 0.1.0
   */
  void tileset_selection_changed(core::position topLeft,
                                 core::position bottomRight);

 public slots:
  void selected_map(map_id map);

 private:
  owner<Ui::tileset_content_page*> m_ui;
  std::optional<map_id> m_currentMap;
  tilemap_map m_mapTabs;

  /**
   * @brief Indicates whether or not there is a tab associated with the
   * specified ID.
   *
   * @param id the ID to look for.
   *
   * @return `true` if there is a tab associated with the ID; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto has_tab(tileset_id id) const -> bool;

  /**
   * @brief Returns the tab associated with the specified index.
   *
   * @param index the index of the desired tab.
   *
   * @return a pointer to the found tab; `nullptr` if no tab was found.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tab_from_index(int index) -> tileset_tab*;

  /**
   * @brief Returns the index of the tab associated with the specified index.
   *
   * @param id the ID associated with the desired tab.
   *
   * @return the index of the tab associated with the ID; `std::nullopt` if no
   * such tab was found.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto index_of(tileset_id id) const -> std::optional<int>;

  [[nodiscard]] auto current_tab() const -> const tileset_map&
  {
    return m_mapTabs.at(m_currentMap.value());
  }

  [[nodiscard]] auto current_tab() -> tileset_map&
  {
    return m_mapTabs.at(m_currentMap.value());
  }

 private slots:
  void handle_remove_tab(int index);

  void handle_tab_changed(int index);
};

}  // namespace tactile::gui
