#pragma once

#include <QWidget>
#include <memory>

#include "tactile_fwd.hpp"
#include "tactile_types.hpp"

namespace Ui {
class TilesetContentPageUI;
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
                   int tileWidth,
                   int tileHeight,
                   const QString& tabName) noexcept;

  void remove_tileset(tileset_id id) noexcept;

  [[nodiscard]] auto empty() const noexcept -> bool;

 signals:
  void request_add_tileset();  // FIXME seemingly unused

 private:
  owner<Ui::TilesetContentPageUI*> m_ui;
  std::unordered_map<tileset_id, tileset_tab*> m_tabs;

  //  [[nodiscard]] bool contains_tileset(int id) const noexcept;
};

}  // namespace tactile::gui
