#include "tileset_tab.hpp"

#include <QApplication>
#include <QGridLayout>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>

#include "tileset_image_widget.hpp"

namespace tactile {

TilesetTab::TilesetTab(tileset_id id,
                       const core::Tileset& tileset,
                       QWidget* parent)
    : QWidget{parent}
    , mId{id}
    , mName{tileset.name()}
{
  setObjectName("tileset_tab");

  mLayout = new QGridLayout{this};
  mLayout->setContentsMargins(0, 0, 0, 0);

  mScrollArea = new QScrollArea{this};
  mScrollArea->setBackgroundRole(QPalette::ColorRole::Dark);

  mImageWidget = new TilesetImageWidget{tileset, this};
  mScrollArea->setWidget(mImageWidget);

  mLayout->addWidget(mScrollArea);

  setLayout(mLayout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  connect(mImageWidget,
          &TilesetImageWidget::S_SetTilesetSelection,
          this,
          &TilesetTab::S_SetTilesetSelection);
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
