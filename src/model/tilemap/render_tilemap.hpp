#pragma once

#include <QRect>

#include "core_fwd.hpp"

class QPainter;

namespace tactile::model {

/**
 * @brief Renders a tilemap.
 *
 * @param painter the painter that will be used.
 * @param map the tilemap that will be rendered.
 * @param exposed the exposed rectangle of the scene.
 *
 * @since 0.1.0
 */
void render_tilemap(QPainter& painter,
                    const tilemap& map,
                    const QRectF& exposed);

}  // namespace tactile::model
