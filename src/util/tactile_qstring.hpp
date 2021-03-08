#pragma once

#include <QString>         // QString
#include <QStringLiteral>  // QStringLiteral
#include <ostream>         // ostream
#include <string>          // string

#define TACTILE_QSTRING(str) QStringLiteral(str)

#ifdef QT_DEBUG

// This is mainly used for better messages in unit tests
inline auto operator<<(std::ostream& stream, const QString& string)
    -> std::ostream&
{
  stream << string.toStdString();
  return stream;
}

#endif  // QT_DEBUG
