// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/misc/strong_type.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/** \brief Strong type of layer identifiers. */
TACTILE_STRONG_TYPE(LayerID, int32);

/** \brief Strong type of object identifiers. */
TACTILE_STRONG_TYPE(ObjectID, int32);

/** \brief Strong type for object references. */
TACTILE_STRONG_TYPE(ObjectRef, int32);

}  // namespace tactile
