// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Serializes a mathematical vector for use in save files.
 *
 * \details The returned string is created by placing each component in a string, where
 *          each value is separated with a single `;` character. For example, the vector
 *          `{1, 2, 3, 4}` becomes `"1;2;3;4"`.
 *
 * \param vec the vector to serialize.
 *
 * \return a string that encodes the vector components.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto serialize(const Int2& vec) -> String;

/// \copydoc serialize(const Int2&)
[[nodiscard]]
TACTILE_FOUNDATION_API auto serialize(const Int3& vec) -> String;

/// \copydoc serialize(const Int2&)
[[nodiscard]]
TACTILE_FOUNDATION_API auto serialize(const Int4& vec) -> String;

/// \copydoc serialize(const Int2&)
[[nodiscard]]
TACTILE_FOUNDATION_API auto serialize(const Float2& vec) -> String;

/// \copydoc serialize(const Int2&)
[[nodiscard]]
TACTILE_FOUNDATION_API auto serialize(const Float3& vec) -> String;

/// \copydoc serialize(const Int2&)
[[nodiscard]]
TACTILE_FOUNDATION_API auto serialize(const Float4& vec) -> String;

}  // namespace tactile
