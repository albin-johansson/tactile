#include "tileset_tab.h"

#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

#include "tactile_types.h"
#include "tileset_image_widget.h"
#include "widget_size_policy.h"

namespace tactile {

TilesetTab::TilesetTab(const QImage& image,
                       int tileWidth,
                       int tileHeight,
                       QWidget* parent)
    : QWidget{parent}
{
  if (image.isNull()) {
    throw BadArg{"Can't create tileset tab from null image!"};
  }

  m_imageWidget = TilesetImageWidget::unique(image, tileWidth, tileHeight);

  m_scrollArea = std::make_unique<QScrollArea>();
  m_scrollArea->setBackgroundRole(QPalette::ColorRole::Dark);
  m_scrollArea->setWidget(m_imageWidget.get());

  m_layout = std::make_unique<QGridLayout>();
  m_layout->addWidget(m_scrollArea.get());

  setLayout(m_layout.get());
  set_size_policy(this, QSizePolicy::Expanding);
}

TilesetTab::~TilesetTab() noexcept = default;

}  // namespace tactile
