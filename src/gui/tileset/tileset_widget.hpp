#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "map_id.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"

TACTILE_FORWARD_DECLARE(Ui, tileset_widget)
TACTILE_FORWARD_DECLARE(tactile::gui, tileset_content_page)
TACTILE_FORWARD_DECLARE(tactile::gui, tileset_empty_page)

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
  void ui_rename_tileset(tileset_id id, const QString& name);
  void ui_set_tileset_selection(const core::tileset::selection& selection);

 public slots:
  void added_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  void removed_tileset(tileset_id id);

  void selected_map(map_id id);

 private:
  unique<Ui::tileset_widget> m_ui;
  tileset_content_page* m_contentPage{};
  tileset_empty_page* m_emptyPage{};
  int m_emptyIndex{};
  int m_contentIndex{};
};

}  // namespace tactile::gui
