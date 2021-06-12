#pragma once

#include <QPainter>  // QPainter
#include <QRectF>    // QRectF

#include "forward_declare.hpp"
#include "map_position.hpp"
#include "maybe.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile {

void RenderMap(QPainter& painter,
               const core::MapDocument& document,
               Maybe<core::MapPosition> mousePosition,
               const QRectF& exposed);

}  // namespace tactile
