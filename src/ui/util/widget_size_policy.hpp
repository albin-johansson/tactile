#pragma once

#include <QSizePolicy>

class QWidget;

namespace tactile::ui {

/**
 * Sets the size policy of a widget. This method has no effect if the
 * supplied widget is null.
 *
 * @param widget a pointer to the widget that will be affected.
 * @param policy the size policy that will be used as the vertical and
 * horizontal size policies.
 * @since 0.1.0
 */
void set_size_policy(QWidget* widget, QSizePolicy::Policy policy) noexcept;

/**
 * Sets the size policy of a widget. This method has no effect if the
 * supplied widget is null.
 *
 * @param widget a pointer to the widget that will be affected.
 * @param horizontalPolicy the size policy that will be used as the horizontal
 * policy.
 * @param verticalPolicy the size policy that will be used as the vertical
 * policy.
 * @since 0.1.0
 */
void set_size_policy(QWidget* widget,
                     QSizePolicy::Policy horizontalPolicy,
                     QSizePolicy::Policy verticalPolicy) noexcept;

}  // namespace tactile::ui
