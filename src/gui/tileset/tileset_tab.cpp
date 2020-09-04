#include "tileset_tab.hpp"

#include <qgridlayout.h>
#include <qlabel.h>
#include <qscrollarea.h>

#include "tactile_error.hpp"
#include "tileset_image_widget.hpp"

namespace tactile::gui {

tileset_tab::tileset_tab(const QImage& image,
                         tile_width tileWidth,
                         tile_height tileHeight,
                         QWidget* parent)
    : QWidget{parent}
{
  if (image.isNull()) {
    throw tactile_error{"Can't create tileset tab from null image!"};
  }

  setObjectName("tileset_tab");

  m_layout = new QGridLayout{this};
  m_layout->setMargin(0);

  m_scrollArea = new QScrollArea{};
  m_scrollArea->setBackgroundRole(QPalette::ColorRole::Dark);

  m_imageWidget = new tileset_image_widget{image, tileWidth, tileHeight};
  m_scrollArea->setWidget(m_imageWidget);  // scroll area becomes parent

  m_layout->addWidget(m_scrollArea);  // layout claims ownership of scroll area

  setLayout(m_layout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

tileset_tab::~tileset_tab() noexcept = default;

}  // namespace tactile::gui
