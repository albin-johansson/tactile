#pragma once

#include <QLayout>
#include <QPoint>
#include <QScrollArea>
#include <QWidget>

#include "position.hpp"
#include "tileset.hpp"
#include "tileset_image_widget.hpp"
#include "types.hpp"

namespace tactile::gui {

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

  [[nodiscard]] auto name() const noexcept -> const QString&
  {
    return m_name;
  }

 signals:
  void tileset_selection_changed(core::position topLeft,
                                 core::position bottomRight);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  QScrollArea* m_scrollArea;
  tileset_image_widget* m_imageWidget;
  QLayout* m_layout;
  QPoint m_lastMousePos;
  tileset_id m_id;
  QString m_name;
};

}  // namespace tactile::gui
