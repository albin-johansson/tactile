#pragma once

#include <qwidget.h>

#include <optional>  // optional
#include <vector>    // vector

#include "fwd.hpp"
#include "position.hpp"
#include "types.hpp"

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
   * @brief Adds a tileset tab that represents the supplied tileset.
   *
   * @pre `id` must not be used by another tileset tab.
   * @pre `image` must refer to a non-null image.
   *
   * @param image the image associated with the tileset.
   * @param id the ID associated with the tileset.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   * @param tabName the name of the tileset tab.
   *
   * @since 0.1.0
   */
  void add_tileset(const QImage& image,
                   tileset_id id,
                   tile_width tileWidth,
                   tile_height tileHeight,
                   const QString& tabName);

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

 private:
  using const_iterator = std::vector<tileset_tab*>::const_iterator;
  owner<Ui::tileset_content_page*> m_ui;
  std::vector<tileset_tab*> m_tabs;

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
   * @brief Attempts to find the tab associated with the specified ID.
   *
   * @param id the ID to look for.
   *
   * @return an iterator to the tab that was found; points one past the end of
   * the tab collection if no such tab was found.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto find_tab(tileset_id id) const -> const_iterator;

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

 private slots:
  void handle_remove_tab(int index);

  void handle_tab_changed(int index);
};

}  // namespace tactile::gui
