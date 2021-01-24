#pragma once

#include <gtest/gtest.h>

#define TACTILE_DEFINE_TEST_P(Class, Type)          \
  class Class : public testing::TestWithParam<Type> \
  {};                                               \
  TEST_P(Class, Class##Test)  // NOLINT

#define TACTILE_REGISTER_TEST_P(Class, Values) \
  INSTANTIATE_TEST_SUITE_P(Class##s, Class, Values)  // NOLINT
