#pragma once

#include <QWidget>

#include "czstring.hpp"
#include "position.hpp"
#include "tileset_content_page.hpp"
#include "tileset_empty_page.hpp"

namespace Ui {
class tileset_widget;
}

namespace tactile::gui {

class tileset_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit tileset_widget(QWidget* parent = nullptr);

  ~tileset_widget() noexcept override;

 signals:
  void ui_add_tileset();
  void ui_remove_tileset(tileset_id id);
  void ui_select_tileset(tileset_id id);
  void ui_set_tileset_selection(const core::tileset::selection& selection);

 public slots:
  void added_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  void removed_tileset(tileset_id id)
  {
    m_contentPage->removed_tileset(id);
  }

  void selected_map(map_id map)
  {
    m_contentPage->selected_map(map);
  }

 private:
  Ui::tileset_widget* m_ui;
  tileset_content_page* m_contentPage{};
  tileset_empty_page* m_emptyPage{};
  int m_emptyIndex{};
  int m_contentIndex{};
};

}  // namespace tactile::gui
