#include "tileset_tab.hpp"

#include <QApplication>  // QApplication
#include <QGridLayout>   // QGridLayout
#include <QMouseEvent>   // QMouseEvent
#include <QScrollArea>   // QScrollArea
#include <QScrollBar>    // QScrollBar

#include "tileset.hpp"
#include "tileset_image_widget.hpp"

namespace tactile {

TilesetTab::TilesetTab(tileset_id id,
                       const core::Tileset& tileset,
                       QWidget* parent)
    : QWidget{parent}
    , mId{id}
    , mName{tileset.Name()}
    , mLayout{new QGridLayout{this}}
    , mScrollArea{new QScrollArea{this}}
{
  setObjectName(QStringLiteral(u"TilesetTab"));

  mLayout->setContentsMargins(0, 0, 0, 0);
  mScrollArea->setBackgroundRole(QPalette::ColorRole::Dark);

  mImageWidget = new TilesetImageWidget{tileset, this};
  mScrollArea->setWidget(mImageWidget);

  mLayout->addWidget(mScrollArea);

  setLayout(mLayout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  // clang-format off
  connect(mImageWidget, &TilesetImageWidget::S_SetTilesetSelection, this, &TilesetTab::S_SetTilesetSelection);
  // clang-format on
}

TilesetTab::~TilesetTab() noexcept = default;

void TilesetTab::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  mLastMousePos = event->pos();

  if (event->buttons() & Qt::MiddleButton)
  {
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  }
}

void TilesetTab::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);

  const auto pos = event->pos();

  if (event->buttons() & Qt::MiddleButton)
  {
    auto* vbar = mScrollArea->verticalScrollBar();
    auto* hbar = mScrollArea->horizontalScrollBar();
    hbar->setValue(hbar->value() - pos.x() + mLastMousePos.x());
    vbar->setValue(vbar->value() - pos.y() + mLastMousePos.y());
  }

  mLastMousePos = pos;
}

void TilesetTab::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
  QApplication::restoreOverrideCursor();
}

}  // namespace tactile
