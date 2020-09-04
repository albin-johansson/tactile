#pragma once

#include <qwidget.h>

#include <memory>

#include "tactile_fwd.hpp"
#include "types.hpp"

namespace Ui {
class TilesetWidgetUI;
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

  void remove_tileset(tileset_id id);

 signals:
  void request_new_tileset();

 private:
  owner<Ui::TilesetWidgetUI*> m_ui;
  tileset_content_page* m_contentPage;
  tileset_empty_page* m_emptyPage;
  int m_emptyIndex;
  int m_contentIndex;
};

}  // namespace tactile::gui
