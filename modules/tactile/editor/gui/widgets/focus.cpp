#include "focus.hpp"

#include "tactile/editor/gui/widgets/layers/layer_dock.hpp"
#include "tactile/editor/gui/widgets/properties/properties_dock.hpp"
#include "tactile/editor/gui/widgets/tilesets/tileset_dock.hpp"
#include "tactile/editor/gui/widgets/toolbar/toolbar.hpp"
#include "tactile/editor/gui/widgets/viewport/viewport_widget.hpp"

namespace Tactile {

auto IsEditorFocused() -> bool
{
  return IsViewportFocused() || IsPropertyDockFocused() || IsToolbarFocused() ||
         IsTilesetDockFocused() || IsLayerDockFocused();
}

}  // namespace Tactile
