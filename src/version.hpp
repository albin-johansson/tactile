#pragma once

#include <QString>  // QString

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
[[nodiscard]] inline auto Version() noexcept -> QString
{
  return QStringLiteral(u"0.2.0");
}

}  // namespace tactile
