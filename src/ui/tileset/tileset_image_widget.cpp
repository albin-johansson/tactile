#include "tileset_image_widget.h"

#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QRubberBand>

#include "tactile_types.h"

namespace tactile {

struct TilesetData final {
  int tileWidth = 0;
  int tileHeight = 0;
};

class ImageLabel final : public QLabel {
 public:
  explicit ImageLabel(const QImage& image,
                      TilesetData data,
                      QWidget* parent = nullptr)
      : QLabel{parent}
  {
    if (image.isNull()) {
      throw BadArg{"Can't create tileset image widget from null image!"};
    }
    setPixmap(QPixmap::fromImage(image));

    m_width = image.width();
    m_height = image.height();

    m_maxX = m_width - 1;
    m_maxY = m_height - 1;

    m_tilesetData = data;
    m_nRows = m_height / data.tileHeight;
    m_nCols = m_width / data.tileWidth;
  }

  ~ImageLabel() {}

 protected:
  void paintEvent(QPaintEvent* event) override
  {
    QLabel::paintEvent(event);

    QPainter painter{this};

    for (auto r = 0; r < m_nRows; ++r) {
      for (auto c = 0; c < m_nCols; ++c) {
        const auto x = c * m_tilesetData.tileWidth;
        const auto y = r * m_tilesetData.tileHeight;
        painter.drawLine(x, 0, x, m_width);
        painter.drawLine(0, y, m_height, y);
      }
    }

    {
      // renders the bottom and right-hand end lines
      painter.drawLine(0, m_maxY, m_width, m_maxY);
      painter.drawLine(m_maxX, 0, m_maxX, m_height);
    }
  }

 private:
  int m_width;
  int m_height;
  int m_maxX;
  int m_maxY;
  int m_nRows;
  int m_nCols;
  TilesetData m_tilesetData;
};

TilesetImageWidget::TilesetImageWidget(const QImage& image, QWidget* parent)
    : QWidget{parent}
{
  if (image.isNull()) {
    throw BadArg{"Can't create tileset image widget from null image!"};
  }

  // FIXME add tileset data parameter to this ctor
  m_imageLabel = std::make_unique<ImageLabel>(image, TilesetData{32, 32});

  m_layout = std::make_unique<QGridLayout>();
  m_layout->addWidget(m_imageLabel.get());

  setLayout(m_layout.get());

  m_rubberBand =
      std::make_unique<QRubberBand>(QRubberBand::Rectangle, m_imageLabel.get());
}

TilesetImageWidget::~TilesetImageWidget() noexcept = default;

void TilesetImageWidget::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  if (event->buttons() & Qt::MouseButton::LeftButton &&
      !m_rubberBand->isVisible()) {
    m_origin = event->pos();

    m_rubberBand->setGeometry(QRect{m_origin, QSize()});
    m_rubberBand->show();
  }
}

void TilesetImageWidget::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    m_rubberBand->setGeometry(QRect{m_origin, event->pos()}.normalized());
  }
}

void TilesetImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);

  if (event->button() == Qt::MouseButton::LeftButton) {
    m_rubberBand->hide();
  }
  // TODO compute the selection
}

}  // namespace tactile
