#include "tile_sheet_tab.h"

#include <QGridLayout>
#include <QLabel>

#include "tactile_types.h"
#include "tile_sheet_image_widget.h"
#include "tile_sheet_scroll_area.h"
#include "widget_size_policy.h"

namespace tactile {

TileSheetTab::TileSheetTab(const QImage& image, QWidget* parent)
    : QWidget{parent}
{
  if (image.isNull()) {
    throw BadArg{"Can't create tile sheet tab from null image!"};
  }

  m_imageWidget = std::make_unique<TileSheetImageWidget>(image);

  m_scrollArea = std::make_unique<TileSheetScrollArea>();
  m_scrollArea->setBackgroundRole(QPalette::ColorRole::Dark);
  m_scrollArea->setWidget(m_imageWidget.get());

  m_layout = std::make_unique<QGridLayout>();
  m_layout->addWidget(m_scrollArea.get());

  setLayout(m_layout.get());
  set_size_policy(this, QSizePolicy::Expanding);
}

TileSheetTab::~TileSheetTab() noexcept = default;

}  // namespace tactile
