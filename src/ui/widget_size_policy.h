#pragma once

#include <QSizePolicy>

class QWidget;

namespace tactile {

void set_size_policy(QWidget* widget, QSizePolicy::Policy policy) noexcept;

void set_size_policy(QWidget* widget,
                     QSizePolicy::Policy horizontalPolicy,
                     QSizePolicy::Policy verticalPolicy) noexcept;

}  // namespace tactile
