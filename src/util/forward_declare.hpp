#pragma once

#define TACTILE_FORWARD_DECLARE(ns, clazz) \
  namespace ns {                           \
  class clazz;                             \
  }

#define TACTILE_FORWARD_DECLARE_UI(clazz) \
  namespace Ui {                          \
  class clazz;                            \
  }