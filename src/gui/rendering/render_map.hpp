#pragma once

#include "core_fwd.hpp"
#include "map_position.hpp"
#include "maybe.hpp"

class QPainter;
class QRectF;

namespace tactile {

void RenderMap(QPainter& painter,
               const core::MapDocument& document,
               Maybe<core::MapPosition> mousePosition,
               const QRectF& exposed);

}  // namespace tactile
