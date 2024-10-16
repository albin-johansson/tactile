// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>      // size_t
#include <expected>     // expected
#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // pair

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class IDocumentVisitor;
class Attribute;

/**
 * A read-only view of a component definition.
 */
class IComponentView
{
 public:
  TACTILE_INTERFACE_CLASS(IComponentView);

  /**
   * Inspects the component.
   *
   * \param visitor The visitor to use.
   *
   * \return
   * Nothing if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto accept(IDocumentVisitor& visitor) const -> std::expected<void, ErrorCode> = 0;

  /**
   * Returns the name of the component.
   *
   * \return
   * A component name.
   */
  [[nodiscard]]
  virtual auto get_name() const -> std::string_view = 0;

  /**
   * Returns an attribute in the component.
   *
   * \param index The attribute index.
   *
   * \return
   * An attribute key and value pair.
   */
  [[nodiscard]]
  virtual auto get_attribute(std::size_t index) const
      -> std::pair<const std::string&, const Attribute&> = 0;

  /**
   * Returns the number of attributes in the component.
   *
   * \return
   * An attribute count.
   */
  [[nodiscard]]
  virtual auto attribute_count() const -> std::size_t = 0;
};

}  // namespace tactile
