#include "tileset_tab.h"

#include <QGridLayout>
#include <QLabel>

#include "tactile_types.h"
#include "tileset_image_widget.h"
#include "tileset_scroll_area.h"
#include "widget_size_policy.h"

namespace tactile {

TilesetTab::TilesetTab(const QImage& image, QWidget* parent) : QWidget{parent}
{
  if (image.isNull()) {
    throw BadArg{"Can't create tileset tab from null image!"};
  }

  m_imageWidget = std::make_unique<TilesetImageWidget>(image);

  m_scrollArea = std::make_unique<TilesetScrollArea>();
  m_scrollArea->setBackgroundRole(QPalette::ColorRole::Dark);
  m_scrollArea->setWidget(m_imageWidget.get());

  m_layout = std::make_unique<QGridLayout>();
  m_layout->addWidget(m_scrollArea.get());

  setLayout(m_layout.get());
  set_size_policy(this, QSizePolicy::Expanding);
}

TilesetTab::~TilesetTab() noexcept = default;

}  // namespace tactile
