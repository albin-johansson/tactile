#pragma once

#include <QPoint>
#include <QString>
#include <QWidget>

#include "tileset.hpp"
#include "tileset_id.hpp"

class QScrollArea;

namespace tactile::gui {

class tileset_image_widget;

class tileset_tab final : public QWidget
{
  Q_OBJECT

 public:
  explicit tileset_tab(tileset_id id,
                       const core::tileset& tileset,
                       QWidget* parent = nullptr);

  ~tileset_tab() noexcept override;

  [[nodiscard]] auto id() const noexcept -> tileset_id
  {
    return m_id;
  }

  [[nodiscard]] auto name() const -> const QString&
  {
    return m_name;
  }

 signals:
  void set_tileset_selection(const core::tileset::selection& selection);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  tileset_id m_id;
  QString m_name;
  QLayout* m_layout;
  QScrollArea* m_scrollArea;
  tileset_image_widget* m_imageWidget;
  QPoint m_lastMousePos;
};

}  // namespace tactile::gui
