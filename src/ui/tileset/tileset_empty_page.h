#pragma once
#include <QWidget>

namespace Ui {
class TilesetEmptyPageUI;
}

namespace tactile {

class TilesetEmptyPage final : public QWidget {
  Q_OBJECT

 public:
  explicit TilesetEmptyPage(QWidget* parent = nullptr);

  ~TilesetEmptyPage() noexcept override;

 signals:
  void s_requested_tile_sheet();

 private:
  Ui::TilesetEmptyPageUI* m_ui;
};

}  // namespace tactile
