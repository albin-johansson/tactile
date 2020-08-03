#pragma once

#include <QWidget>
#include <memory>

class QImage;
class QLayout;
class QScrollArea;

namespace tactile::ui {

class TilesetImageWidget;

class TilesetTab final : public QWidget {
 public:
  explicit TilesetTab(const QImage& image,
                      int tileWidth,
                      int tileHeight,
                      QWidget* parent = nullptr);

  ~TilesetTab() noexcept override;

 private:
  std::unique_ptr<QScrollArea> m_scrollArea;
  std::unique_ptr<TilesetImageWidget> m_imageWidget;
  std::unique_ptr<QLayout> m_layout;
};

}  // namespace tactile::ui
