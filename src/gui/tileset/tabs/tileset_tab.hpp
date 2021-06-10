#pragma once

#include <QPoint>       // QPoint
#include <QScrollArea>  // QScrollArea
#include <QString>      // QString
#include <QWidget>      // QWidget

#include "forward_declare.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"

TACTILE_FORWARD_DECLARE(tactile, TilesetImageWidget)

namespace tactile {

class TilesetTab final : public QWidget
{
  Q_OBJECT

 public:
  explicit TilesetTab(tileset_id id,
                      const core::Tileset& tileset,
                      QWidget* parent = nullptr);

  ~TilesetTab() noexcept override;

  [[nodiscard]] auto Id() const noexcept -> tileset_id
  {
    return mId;
  }

  [[nodiscard]] auto Name() const -> const QString&
  {
    return mName;
  }

 signals:
  void S_SetTilesetSelection(const core::tileset_selection& selection);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  tileset_id mId;
  QString mName;
  QLayout* mLayout{};
  QScrollArea* mScrollArea{};
  TilesetImageWidget* mImageWidget{};
  QPoint mLastMousePos;
};

}  // namespace tactile
