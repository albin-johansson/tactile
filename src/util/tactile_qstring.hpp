#pragma once

#include <QStringLiteral>

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
