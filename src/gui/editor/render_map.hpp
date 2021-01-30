#pragma once

#include <QRectF>  // QRectF

#include "maybe.hpp"
#include "position.hpp"

class QPainter;

namespace tactile::core {
class map_document;
}

namespace tactile::gui {

void render_map(QPainter& painter,
                const core::map_document& document,
                maybe<core::position>,
                const QRectF& exposed);

}  // namespace tactile::gui
