#pragma once

#include <QWidget>
#include <memory>

#include "tactile_fwd.hpp"

class QImage;
class QLayout;
class QScrollArea;

namespace tactile::ui {

class tileset_tab final : public QWidget {
 public:
  explicit tileset_tab(const QImage& image,
                       int tileWidth,
                       int tileHeight,
                       QWidget* parent = nullptr);

  ~tileset_tab() noexcept override;

 private:
  std::unique_ptr<QScrollArea> m_scrollArea;
  std::unique_ptr<tileset_image_widget> m_imageWidget;
  std::unique_ptr<QLayout> m_layout;
};

}  // namespace tactile::ui
