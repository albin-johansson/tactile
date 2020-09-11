#pragma once

#include <qwidget.h>

#include <memory>

#include "fwd.hpp"
#include "position.hpp"
#include "types.hpp"

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

  void add_tileset(const QImage& image,
                   tileset_id id,
                   tile_width tileWidth,
                   tile_height tileHeight,
                   const QString& tabName);

 signals:
  void request_new_tileset();

  void selected_tileset(tileset_id id);

  void removed_tileset(tileset_id id);

  void tileset_selection_changed(core::position topLeft,
                                 core::position bottomRight);

 private:
  owner<Ui::tileset_widget*> m_ui;
  tileset_content_page* m_contentPage;
  tileset_empty_page* m_emptyPage;
  int m_emptyIndex;
  int m_contentIndex;
};

}  // namespace tactile::gui
