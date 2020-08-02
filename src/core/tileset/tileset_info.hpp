#pragma once

#include "smart_pointers.hpp"

class QImage;

namespace tactile {

/**
 * The <code>TilesetInfo</code> struct provides information about a tileset.
 *
 * @since 0.1.0
 */
struct TilesetInfo final {
  Shared<QImage> image = nullptr;
  int id = 0;
  int tileWidth = 1;
  int tileHeight = 1;
};

}  // namespace tactile
