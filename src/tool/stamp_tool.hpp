#pragma once

#include <optional>

#include "fwd.hpp"
#include "position.hpp"
#include "tileset.hpp"
#include "tool.hpp"

namespace tactile {

class stamp_tool final : public tool
{
 public:
  explicit stamp_tool(core::model* model);

  void pressed(QMouseEvent* event, const QPointF& mapPosition) override;

  void moved(QMouseEvent* event, const QPointF& mapPosition) override;

  void released(QMouseEvent* event, const QPointF& mapPosition) override;

  void entered(QEvent* event) override;

  void exited(QEvent* event) override;

 private:
  core::model* m_model{};

  [[nodiscard]] auto translate_mouse_position(const QPoint& mousePosition,
                                              const QPointF& mapPosition) const
      -> std::optional<core::position>;

  void set_tiles(const core::position& position, const core::tileset& tileset);
};

}  // namespace tactile
