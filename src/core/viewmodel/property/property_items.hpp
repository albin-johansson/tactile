#pragma once

#include <QStandardItem>

#include "item_type.hpp"
#include "property_item_role.hpp"

namespace tactile::viewmodel {

class string_item final : public QStandardItem
{
 public:
  string_item() : QStandardItem{}
  {
    setEditable(true);
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
  {}

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(item_type::integer);
  }
};

class float_item final : public QStandardItem
{
 public:
  float_item() : QStandardItem{}
  {}

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
  {}

  [[nodiscard]] auto type() const -> int override
  {
    return static_cast<int>(item_type::file);
  }
};

}  // namespace tactile::viewmodel
