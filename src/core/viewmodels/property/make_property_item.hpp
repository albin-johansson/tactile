#pragma once

#include <QStandardItem>

#include "property.hpp"

namespace tactile::vm {

[[nodiscard]] auto MakePropertyItem(const core::Property& property)
    -> QStandardItem*;

}  // namespace tactile::vm
