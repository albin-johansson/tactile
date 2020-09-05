#pragma once

#include <qwidget.h>

#include <vector>

#include "fwd.hpp"
#include "position.hpp"
#include "types.hpp"

namespace Ui {
class tileset_content_page;
}

namespace tactile::gui {

class tileset_content_page final : public QWidget
{
  Q_OBJECT

 public:
  explicit tileset_content_page(QWidget* parent = nullptr);

  ~tileset_content_page() noexcept override;

  void add_tileset(const QImage& image,
                   tileset_id id,
                   tile_width tileWidth,
                   tile_height tileHeight,
                   const QString& tabName);

  void remove_tileset(tileset_id id);

  [[nodiscard]] auto empty() const -> bool;

 signals:
  void request_add_tileset();

  void selected_tileset(tileset_id id);

  void removed_tileset(tileset_id id);

  void tileset_selection_changed(core::position topLeft,
                                 core::position bottomRight);

  void switch_to_empty_page();

 private:
  using const_iterator = std::vector<tileset_tab*>::const_iterator;
  owner<Ui::tileset_content_page*> m_ui;
  std::vector<tileset_tab*> m_tabs;

  [[nodiscard]] auto has_tab(tileset_id id) const -> bool;

  [[nodiscard]] auto find_tab(tileset_id id) const -> const_iterator;

  [[nodiscard]] auto tab_from_index(int index) -> tileset_tab*;

 private slots:
  void handle_remove_tab(int index);

  void handle_tab_changed(int index);
};

}  // namespace tactile::gui
