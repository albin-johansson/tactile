#pragma once

#include <QStandardItem>

#include "item_type.hpp"
#include "property_item_role.hpp"
#include "tactile_qstring.hpp"

namespace tactile::viewmodel {

class string_item final : public QStandardItem
{
 public:
  string_item() : QStandardItem{}
  {
    setEditable(true);
    setToolTip(TACTILE_QSTRING(u"(string)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(item_type::string);
  }
};

class int_item final : public QStandardItem
{
 public:
  int_item() : QStandardItem{}
  {
    setToolTip(TACTILE_QSTRING(u"(int)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(item_type::integer);
  }
};

class float_item final : public QStandardItem
{
 public:
  float_item() : QStandardItem{}
  {
    setToolTip(TACTILE_QSTRING(u"(float)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(item_type::floating);
  }
};

class bool_item final : public QStandardItem
{
 public:
  bool_item() : QStandardItem{}
  {
    setEditable(false);
    setCheckable(true);
    setToolTip(TACTILE_QSTRING(u"(bool)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(item_type::boolean);
  }
};

class object_item final : public QStandardItem
{
 public:
  object_item() : QStandardItem{}
  {
    setEditable(false);
    setToolTip(TACTILE_QSTRING(u"(object)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(item_type::object);
  }
};

class color_item final : public QStandardItem
{
 public:
  color_item() : QStandardItem{}
  {
    setEditable(false);
    setToolTip(TACTILE_QSTRING(u"(color)"));
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(item_type::color);
  }
};

class file_item final : public QStandardItem
{
 public:
  file_item() : QStandardItem{}
  {
    setToolTip(TACTILE_QSTRING(u"(file)"));
    setEditable(false);
  }

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(item_type::file);
  }
};

}  // namespace tactile::viewmodel
