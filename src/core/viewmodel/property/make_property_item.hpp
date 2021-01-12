#pragma once

#include <QStandardItem>

#include "property.hpp"

namespace tactile::vm {

[[nodiscard]] auto make_property_item(const core::property& property)
    -> QStandardItem*;

}
