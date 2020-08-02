#include "widget_size_policy.hpp"

#include <QWidget>
#include <catch.hpp>

using namespace tactile;

TEST_CASE("set_size_policy1", "[set_size_policy]")
{
  QWidget widget;

  const auto policy = QSizePolicy::Expanding;
  set_size_policy(&widget, policy);

  CHECK(widget.sizePolicy().horizontalPolicy() == policy);
  CHECK(widget.sizePolicy().verticalPolicy() == policy);

  CHECK_NOTHROW(set_size_policy(nullptr, QSizePolicy::Maximum));
}

TEST_CASE("set_size_policy2", "[set_size_policy]")
{
  QWidget widget;

  const auto verticalPolicy = QSizePolicy::Preferred;
  const auto horizontalPolicy = QSizePolicy::Minimum;

  set_size_policy(&widget, horizontalPolicy, verticalPolicy);

  CHECK(widget.sizePolicy().horizontalPolicy() == horizontalPolicy);
  CHECK(widget.sizePolicy().verticalPolicy() == verticalPolicy);

  CHECK_NOTHROW(
      set_size_policy(nullptr, QSizePolicy::Maximum, QSizePolicy::Minimum));
}