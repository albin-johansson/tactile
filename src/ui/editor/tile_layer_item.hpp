#pragma once

#include <QGraphicsItem>

#include "core.hpp"

namespace tactile::ui {

class tile_layer_item final : public QGraphicsItem {
 public:
  explicit tile_layer_item(model::core* core,
                           QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  [[nodiscard]] auto boundingRect() const -> QRectF override;

//  [[nodiscard]] auto layer() noexcept -> model::tile_layer*;
//
//  [[nodiscard]] auto layer() const noexcept -> const model::tile_layer*;

 private:
  model::core* m_core{};
};

}  // namespace tactile::ui
