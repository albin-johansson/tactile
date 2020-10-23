#pragma once

#include <QWidget>
#include <map>       // map
#include <optional>  // optional

#include "map_id.hpp"
#include "position.hpp"
#include "tileset_tab.hpp"
#include "tileset_tab_manager.hpp"
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
   * @brief Indicates whether or not there are any tileset tabs.
   *
   * @return `true` if there are no tileset tabs; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto empty() const -> bool;

 signals:
  void switch_to_empty_page();
  void switch_to_content_page();
  void ui_add_tileset();
  void ui_select_tileset(tileset_id id);
  void ui_remove_tileset(tileset_id id);
  void ui_set_tileset_selection(const core::tileset::selection& selection);

 public slots:
  void selected_map(map_id map);

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
  void added_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  /**
   * @brief Removes the tileset tab associated with the specified tileset.
   *
   * @pre `id` must be associated with an existing tileset tab.
   *
   * @param id the ID associated with the tileset tab which will be removed.
   *
   * @since 0.1.0
   */
  void removed_tileset(tileset_id id);

 private:
  Ui::tileset_content_page* m_ui{};
  std::optional<map_id> m_currentMap;
  std::map<map_id, tileset_tab_manager> m_tabManagers;

  void switch_to(map_id map);

  void add_corner_button();

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

  [[nodiscard]] auto current_manager() const -> const tileset_tab_manager&
  {
    return m_tabManagers.at(m_currentMap.value());
  }

  [[nodiscard]] auto current_manager() -> tileset_tab_manager&
  {
    return m_tabManagers.at(m_currentMap.value());
  }
};

}  // namespace tactile::gui
