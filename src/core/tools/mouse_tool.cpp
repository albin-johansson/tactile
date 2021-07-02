#include "mouse_tool.hpp"

#include "core/model.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

AMouseTool::AMouseTool(NotNull<Model*> model) : mModel{model}
{
  if (!mModel)
  {
    throw TactileError{"Cannot create mouse tool from null model!"};
  }
}

auto AMouseTool::GetDocument() -> MapDocument*
{
  return mModel->GetActiveDocument();
}

auto AMouseTool::GetDocument() const -> const MapDocument*
{
  return mModel->GetActiveDocument();
}

auto AMouseTool::GetTileset() const -> const Tileset*
{
  if (const auto* document = GetDocument())
  {
    return document->GetTilesets().GetActiveTileset();
  }
  else
  {
    return nullptr;
  }
}

}  // namespace Tactile
