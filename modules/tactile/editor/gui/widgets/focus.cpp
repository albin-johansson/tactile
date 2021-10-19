#include "focus.hpp"

#include "editor/gui/widgets/layers/layer_dock.hpp"
#include "editor/gui/widgets/properties/properties_dock.hpp"
#include "editor/gui/widgets/tilesets/tileset_dock.hpp"
#include "editor/gui/widgets/toolbar/toolbar.hpp"
#include "editor/gui/widgets/viewport/viewport_widget.hpp"

namespace Tactile {

auto IsEditorFocused() -> bool
{
  return IsViewportFocused() || IsPropertyDockFocused() || IsToolbarFocused() ||
         IsTilesetDockFocused() || IsLayerDockFocused();
}

}  // namespace Tactile
