#pragma once

#include <QString>  // QString

#include "tactile_qstring.hpp"

namespace tactile {

/**
 * \brief Returns a string that indicates the current version of the
 * application.
 *
 * \return a version string, on the form `"X.Y.Z"`, where X, Y and Z correspond
 * to major, minor and patch respectively.
 *
 * \since 0.1.0
 */
[[nodiscard]] inline auto version() noexcept -> QString
{
  return TACTILE_QSTRING(u"0.2.0");
}

}  // namespace tactile
