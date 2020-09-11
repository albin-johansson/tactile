#include "abstract_tool.hpp"

#include "model.hpp"
#include "tactile_error.hpp"

namespace tactile {

abstract_tool::abstract_tool(core::model* model) : m_model{model}
{
  if (!m_model) {
    throw tactile_error{"Cannot create tool from null model!"};
  }
}

auto abstract_tool::get_model() noexcept -> core::model*
{
  return m_model;
}

auto abstract_tool::get_model() const noexcept -> const core::model*
{
  return m_model;
}

auto abstract_tool::translate_mouse_position(const QPoint& mousePosition,
                                             const QPointF& mapPosition) const
    -> std::optional<core::position>
{
  const auto x = mousePosition.x() - mapPosition.x();
  const auto y = mousePosition.y() - mapPosition.y();

  if (x < 0 || y < 0) {
    return std::nullopt;
  }

  auto* map = m_model->current_raw_map();
  Q_ASSERT(map);

  const auto tileSize = map->get_tile_size().get();

  const core::row row{static_cast<int>(y) / tileSize};
  const core::col col{static_cast<int>(x) / tileSize};

  if (map->in_bounds(row, col)) {
    return core::position{row, col};
  } else {
    return std::nullopt;
  }
}

}  // namespace tactile
