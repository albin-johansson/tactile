#pragma once

#include <memory>

class QImage;

namespace tactile {

/**
 * The <code>TilesetInfo</code> struct provides information about a tileset.
 *
 * @since 0.1.0
 */
struct [[deprecated]] TilesetInfo final {
  std::shared_ptr<QImage> image{};
  int id{0};
  int tileWidth{1};
  int tileHeight{1};
};

}  // namespace tactile
