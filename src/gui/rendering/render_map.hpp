#pragma once

#include <QPainter>  // QPainter
#include <QRectF>    // QRectF

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "position.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile {

void RenderMap(QPainter& painter,
               const core::MapDocument& document,
               maybe<core::Position> mousePosition,
               const QRectF& exposed);

}  // namespace tactile
