#include "tile_sheet_tab.h"

#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

#include "tactile_types.h"
#include "widget_size_policy.h"

namespace tactile {

TileSheetTab::TileSheetTab(const QImage& image, QWidget* parent)
    : QWidget{parent}
{
  if (image.isNull()) {
    throw BadArg{"Can't create tile sheet tab from null image!"};
  }

  m_imageLabel = std::make_unique<QLabel>();
  m_imageLabel->setPixmap(QPixmap::fromImage(image));

  m_scrollArea = std::make_unique<QScrollArea>();
  m_scrollArea->setBackgroundRole(QPalette::Dark);
  m_scrollArea->setWidget(m_imageLabel.get());

  m_layout = std::make_unique<QGridLayout>();
  m_layout->addWidget(m_scrollArea.get());

  setLayout(m_layout.get());
  set_size_policy(this, QSizePolicy::Expanding);
}

TileSheetTab::~TileSheetTab() noexcept = default;

}  // namespace tactile
