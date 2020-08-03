#include "tileset_tab.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

#include "tactile_error.hpp"
#include "tileset_image_widget.hpp"
#include "widget_size_policy.hpp"

namespace tactile::ui {

TilesetTab::TilesetTab(const QImage& image,
                       int tileWidth,
                       int tileHeight,
                       QWidget* parent)
    : QWidget{parent}
{
  if (image.isNull()) {
    throw tactile_error{"Can't create tileset tab from null image!"};
  }

  m_imageWidget = TilesetImageWidget::unique(image, tileWidth, tileHeight);

  m_scrollArea = std::make_unique<QScrollArea>();
  m_scrollArea->setBackgroundRole(QPalette::ColorRole::Dark);
  m_scrollArea->setWidget(m_imageWidget.get());

  m_layout = std::make_unique<QGridLayout>();
  m_layout->setMargin(0);
  m_layout->addWidget(m_scrollArea.get());

  setLayout(m_layout.get());
  set_size_policy(this, QSizePolicy::Expanding);
}

TilesetTab::~TilesetTab() noexcept = default;

}  // namespace tactile::ui
