#include "abstract_tool.hpp"

#include "map_document.hpp"
#include "model.hpp"
#include "tactile_error.hpp"

namespace tactile {

using namespace core;

abstract_tool::abstract_tool(core::model* model) : m_model{model}
{
  if (!m_model)
  {
    throw TactileError{"Cannot create tool from null model!"};
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
    -> maybe<core::Position>
{
  if (const auto* document = m_model->current_document())
  {
    const auto x = mousePosition.x() - mapPosition.x();
    const auto y = mousePosition.y() - mapPosition.y();

    if (x < 0 || y < 0)
    {
      return std::nullopt;
    }

    const auto tileSize = document->CurrentTileSize();

    const Position position{row_t{static_cast<int>(y) / tileSize},
                            col_t{static_cast<int>(x) / tileSize}};

    if (document->InBounds(position))
    {
      return position;
    }
  }
  return std::nullopt;
}

}  // namespace tactile
