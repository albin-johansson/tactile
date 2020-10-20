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
  void ui_requested_tileset();

  void ui_selected_tileset(tileset_id id);

  void ui_removed_tileset(tileset_id id);

  void tileset_selection_changed(core::position topLeft,
                                 core::position bottomRight);

 public slots:
  void select_map(map_id map)
  {
    m_contentPage->select_map(map);
  }

  void add_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  void remove_tileset(tileset_id id)
  {
    m_contentPage->remove_tileset(id, false);
  }

 private:
  Ui::tileset_widget* m_ui;
  tileset_content_page* m_contentPage{};
  tileset_empty_page* m_emptyPage{};
  int m_emptyIndex{};
  int m_contentIndex{};
};

}  // namespace tactile::gui
