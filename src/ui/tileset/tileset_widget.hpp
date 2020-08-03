#pragma once

#include <QWidget>
#include <memory>

#include "tactile_fwd.hpp"
#include "tactile_types.hpp"

namespace Ui {
class TilesetWidgetUI;
}

namespace tactile::ui {

class tileset_widget final : public QWidget {
  Q_OBJECT

 public:
  explicit tileset_widget(QWidget* parent = nullptr);

  ~tileset_widget() noexcept override;

  void add_tileset(const model::tileset_info& info,
                   const QString& tabName) noexcept;

  void remove_tileset(int id) noexcept;

 signals:
  void s_requested_tileset();

 private:
  owner<Ui::TilesetWidgetUI*> m_ui;
  tileset_content_page* m_contentPage;
  tileset_empty_page* m_emptyPage;
  int m_emptyIndex;
  int m_contentIndex;
};

}  // namespace tactile::ui
