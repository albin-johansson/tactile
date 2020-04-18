#pragma once
#include <QWidget>

#include "smart_pointers.h"

namespace Ui {
class TilesetWidgetUI;
}

namespace tactile {

class TilesetContentPage;
class TilesetEmptyPage;

class TilesetWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit TilesetWidget(QWidget* parent = nullptr);

  ~TilesetWidget() noexcept override;

  void add_tileset(int id, const Shared<QImage>& image) noexcept;

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

}  // namespace tactile
