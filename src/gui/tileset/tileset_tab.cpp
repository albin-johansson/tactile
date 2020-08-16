#include "tileset_tab.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

#include "tactile_error.hpp"
#include "tileset_image_widget.hpp"
#include "widget_size_policy.hpp"

namespace tactile::gui {

tileset_tab::tileset_tab(const QImage& image,
                         int tileWidth,
                         int tileHeight,
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
  set_size_policy(this, QSizePolicy::Expanding);
}

tileset_tab::~tileset_tab() noexcept = default;

}  // namespace tactile::gui
