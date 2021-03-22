#include "color_validator.hpp"

namespace tactile::gui {

color_validator::color_validator(QObject* parent)
    : QIntValidator{0, 255, parent}
{}

auto color_validator::validate(QString& string, int&) const -> State
{
  bool ok;

  if (const auto number = string.toInt(&ok); ok)
  {
    if (number >= bottom() && number <= top())
    {
      return Acceptable;
    }
  }

  return Invalid;
}

}  // namespace tactile::gui
