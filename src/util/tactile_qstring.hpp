#pragma once

#include <QString>         // QString
#include <QStringLiteral>  // QStringLiteral
#include <ostream>         // ostream
#include <string>          // string

// This macro is an improved version of the QStringLiteral macro
#define TACTILE_QSTRING(str)                                       \
  ([]() noexcept -> QString {                                      \
    constexpr auto size = sizeof(QT_UNICODE_LITERAL(str)) / 2 - 1; \
                                                                   \
    static const QStaticStringData<size> literal{                  \
        Q_STATIC_STRING_DATA_HEADER_INITIALIZER(size),             \
        QT_UNICODE_LITERAL(str)};                                  \
                                                                   \
    QStringDataPtr holder{literal.data_ptr()};                     \
    QString tmp{holder};                                           \
                                                                   \
    return tmp;                                                    \
  }())

#ifdef QT_DEBUG

// This is mainly used for better messages in unit tests
inline auto operator<<(std::ostream& stream, const QString& string)
    -> std::ostream&
{
  stream << string.toStdString();
  return stream;
}

#endif  // QT_DEBUG
