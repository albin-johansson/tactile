#pragma once

#include <QString>

#include "layer_type.hpp"

namespace tactile::core {

/**
 * \class layer_delegate
 *
 * \brief A delegate meant to be used to implement the basic layer API.
 *
 * \since 0.2.0
 *
 * \headerfile layer_delegate.hpp
 */
class layer_delegate final
{
 public:
  explicit layer_delegate(layer_type type) noexcept;

  void set_visible(bool visible) noexcept;

  void set_opacity(double opacity);

  void set_name(QString name);

  [[nodiscard]] auto type() const noexcept -> layer_type;

  [[nodiscard]] auto visible() const noexcept -> bool;

  [[nodiscard]] auto opacity() const noexcept -> double;

  [[nodiscard]] auto name() const -> const QString&;

 private:
  layer_type m_type;
  QString m_name;
  double m_opacity{1};
  bool m_visible{true};
};

}  // namespace tactile::core
