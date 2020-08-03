#pragma once
#include <QWidget>

namespace Ui {
class TilesetEmptyPageUI;
}

namespace tactile::ui {

class TilesetEmptyPage final : public QWidget {
  Q_OBJECT

 public:
  explicit TilesetEmptyPage(QWidget* parent = nullptr);

  ~TilesetEmptyPage() noexcept override;

 signals:
  void s_requested_tileset();

 private:
  Ui::TilesetEmptyPageUI* m_ui;
};

}  // namespace tactile::ui
