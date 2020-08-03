#pragma once

#include <QWidget>
#include <memory>

#include "core_fwd.hpp"

namespace Ui {
class TilesetWidgetUI;
}

namespace tactile::ui {

class TilesetContentPage;
class TilesetEmptyPage;

class TilesetWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit TilesetWidget(QWidget* parent = nullptr);

  ~TilesetWidget() noexcept override;

  void add_tileset(const TilesetInfo& info, const QString& tabName) noexcept;

  void remove_tileset(int id) noexcept;

 signals:
  void s_requested_tileset();

 private:
  Ui::TilesetWidgetUI* m_ui;
  TilesetContentPage* m_contentPage;
  TilesetEmptyPage* m_emptyPage;
  int m_emptyIndex;
  int m_contentIndex;
};

}  // namespace tactile::ui
