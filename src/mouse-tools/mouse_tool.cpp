#include "mouse_tool.hpp"

#include "map_document.hpp"
#include "model.hpp"
#include "tactile_error.hpp"

namespace tactile {

AMouseTool::AMouseTool(core::Model* model) : mModel{model}
{
  if (!mModel)
  {
    throw TactileError{"Cannot create tool from null model!"};
  }
}

auto AMouseTool::GetModel() noexcept -> core::Model*
{
  return mModel;
}

auto AMouseTool::GetModel() const noexcept -> const core::Model*
{
  return mModel;
}

auto AMouseTool::TranslateMousePosition(const QPoint& mousePosition,
                                        const QPointF& mapPosition) const
    -> Maybe<core::Position>
{
  if (const auto* document = mModel->CurrentDocument())
  {
    const auto x = mousePosition.x() - mapPosition.x();
    const auto y = mousePosition.y() - mapPosition.y();

    if (x < 0 || y < 0)
    {
      return nothing;
    }

    const auto tileSize = document->CurrentTileSize();

    const core::Position position{row_t{static_cast<int>(y) / tileSize},
                                  col_t{static_cast<int>(x) / tileSize}};

    if (document->InBounds(position))
    {
      return position;
    }
  }
  return nothing;
}

}  // namespace tactile
