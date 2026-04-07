// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <utility>

#include "tactile/core/container/string.hpp"
#include "tactile/core/meta/attribute_set.hpp"

namespace tactile {

/// Represents user-defined metadata.
class Metadata final
{
 public:
  /// Sets the name of this context.
  void set_name(String name) { m_name = std::move(name); }

  /// Returns the name of this context.
  auto get_name() const -> const String& { return m_name; }

  /// Returns the properties associated with this context.
  auto get_props() -> AttributeSet& { return m_props; }

  auto get_props() const -> const AttributeSet& { return m_props; }

 private:
  String m_name {};
  AttributeSet m_props {};
};

}  // namespace tactile
