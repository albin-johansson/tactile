#pragma once

#include <QStringLiteral>  // QStringLiteral

// TODO update parser logic for new tiled version

#define TACTILE_TILED_VERSION_LITERAL QStringLiteral(u"1.4.2")
#define TACTILE_TILED_JSON_VERSION_LITERAL QStringLiteral(u"1.4")

namespace tactile {

inline constexpr auto tiled_version = "1.4.2";
inline constexpr auto tiled_xml_version = "1.4";

}  // namespace tactile
