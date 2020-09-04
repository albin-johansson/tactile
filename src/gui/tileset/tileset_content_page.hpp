#pragma once

#include <qwidget.h>

#include <memory>

#include "fwd.hpp"
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
  void request_add_tileset();  // FIXME seemingly unused

 private:
  owner<Ui::tileset_content_page*> m_ui;
  std::unordered_map<tileset_id, tileset_tab*> m_tabs;

  //  [[nodiscard]] bool contains_tileset(int id) const noexcept;
};

}  // namespace tactile::gui
