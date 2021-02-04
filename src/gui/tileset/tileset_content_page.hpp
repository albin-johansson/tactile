#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "map_id.hpp"
#include "maybe.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"
#include "tileset_tab_manager.hpp"
#include "vector_map.hpp"

TACTILE_FORWARD_DECLARE(Ui, tileset_content_page)
TACTILE_FORWARD_DECLARE(tactile::gui, tab_widget)
TACTILE_FORWARD_DECLARE(tactile::gui, tileset_tab)
TACTILE_FORWARD_DECLARE(tactile::gui, tileset_tab_context_menu)

namespace tactile::gui {

/**
 * \class tileset_content_page
 *
 * \brief Represents the page with the tilesets associated with a map.
 *
 * \note This widget is meant to be shown when a map has at least one available
 * tileset.
 *
 * \since 0.1.0
 *
 * \headerfile tileset_content_page.hpp
 */
class tileset_content_page final : public QWidget
{
  Q_OBJECT

 public:
  explicit tileset_content_page(QWidget* parent = nullptr);

  ~tileset_content_page() noexcept override;

  /**
   * \brief Indicates whether or not there are any tileset tabs.
   *
   * \return `true` if there are no tileset tabs; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto is_empty() const -> bool;

 signals:
  void switch_to_empty_page();
  void switch_to_content_page();
  void ui_add_tileset();
  void ui_select_tileset(tileset_id id);
  void ui_remove_tileset(tileset_id id);
  void ui_rename_tileset(tileset_id id, const QString& name);
  void ui_set_tileset_selection(const core::tileset::selection& selection);

 public slots:
  /**
   * \brief Makes the tileset content page display the tilesets associated with
   * the specified map.
   *
   * \param map the ID associated with the selected map.
   *
   * \since 0.1.0
   */
  void selected_map(map_id map);

  /**
   * \brief Adds a tileset tab that represents the supplied tileset.
   *
   * \pre `id` must not have been added before.
   *
   * \param map the ID associated with the map that owns the tileset.
   * \param id the ID associated with the tileset that will be added.
   * \param tileset the tileset that will be added.
   *
   * \since 0.1.0
   */
  void added_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  /**
   * \brief Removes the tileset tab associated with the specified tileset.
   *
   * \pre `id` must be associated with an existing tileset tab.
   *
   * \param id the ID associated with the tileset tab which will be removed.
   *
   * \since 0.1.0
   */
  void removed_tileset(tileset_id id);

 private:
  unique<Ui::tileset_content_page> m_ui;
  tab_widget* m_tabWidget{};
  tileset_tab_context_menu* m_contextMenu{};
  maybe<map_id> m_currentMap;
  vector_map<map_id, tileset_tab_manager> m_tabManagers;

  /**
   * \brief Switches to the tileset tabs associated with the specified map.
   *
   * \param map the ID of the map to switch to.
   *
   * \since 0.1.0
   *
   * \signal `ui_select_tileset`
   */
  void switch_to(map_id map);

  /**
   * \brief Adds the corner button to the tab widget, which is used to add
   * tilesets.
   *
   * \since 0.1.0
   */
  void add_corner_button();

  void trigger_context_menu(const QPoint& pos);

  /**
   * \brief Returns the tab associated with the specified index.
   *
   * \param index the index of the desired tab.
   *
   * \return a pointer to the found tab; `nullptr` if no tab was found.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto tab_from_index(int index) -> tileset_tab*;

  /**
   * \brief Returns the tab manager associated with the current map.
   *
   * \note This function throws if there is no current map *or* if there is no
   * manager associated with the current map.
   *
   * \return the tab manager associated with the current map.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto current_manager() -> tileset_tab_manager&;
};

}  // namespace tactile::gui
