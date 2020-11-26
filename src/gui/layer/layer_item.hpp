#pragma once

#include <QListWidgetItem>

#include "layer_id.hpp"

namespace tactile::gui {

class layer_item final : public QListWidgetItem
{
 public:
  explicit layer_item(const QString& name,
                      layer_id id,
                      QListWidget* parent = nullptr);

  [[nodiscard]] auto layer() const noexcept -> layer_id
  {
    return m_layer;
  }

 private:
  layer_id m_layer;
};

}  // namespace tactile::gui
