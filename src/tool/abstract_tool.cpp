#include "abstract_tool.hpp"

#include "model.hpp"
#include "tactile_error.hpp"

namespace tactile {

using namespace core;

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
  if (auto* document = m_model->current_map_document()) {
    const auto x = mousePosition.x() - mapPosition.x();
    const auto y = mousePosition.y() - mapPosition.y();

    if (x < 0 || y < 0) {
      return std::nullopt;
    }

    const auto& map = document->map_ref();
    const auto tileSize = map.get_tile_size().get();

    const row_t row{static_cast<int>(y) / tileSize};
    const col_t col{static_cast<int>(x) / tileSize};

    if (const position position{row, col}; map.in_bounds(position)) {
      return position;
    }
  }
  return std::nullopt;
}

}  // namespace tactile
