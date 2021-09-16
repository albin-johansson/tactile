#include "focus.hpp"

#include "layers/layer_dock.hpp"
#include "properties/properties_dock.hpp"
#include "tilesets/tileset_dock.hpp"
#include "toolbar/toolbar.hpp"
#include "viewport/viewport_widget.hpp"

namespace Tactile {

auto IsEditorFocused() -> bool
{
  return IsViewportFocused() || IsPropertyDockFocused() || IsToolbarFocused()
         || IsTilesetDockFocused() || IsLayerDockFocused();
}

}  // namespace Tactile
