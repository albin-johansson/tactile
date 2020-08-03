#include "widget_size_policy.hpp"

#include <QWidget>

namespace tactile::ui {

void set_size_policy(QWidget* widget, QSizePolicy::Policy policy) noexcept
{
  set_size_policy(widget, policy, policy);
}

void set_size_policy(QWidget* widget,
                     QSizePolicy::Policy horizontalPolicy,
                     QSizePolicy::Policy verticalPolicy) noexcept
{
  if (!widget) {
    return;
  }

  QSizePolicy policy;
  policy.setHorizontalPolicy(horizontalPolicy);
  policy.setVerticalPolicy(verticalPolicy);
  widget->setSizePolicy(policy);
}

}  // namespace tactile::ui
