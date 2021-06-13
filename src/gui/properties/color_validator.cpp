#include "color_validator.hpp"

#include "string_utils.hpp"

namespace tactile {

ColorValidator::ColorValidator(QObject* parent) : QIntValidator{0, 255, parent}
{
  setObjectName(QStringLiteral(u"ColorValidator"));
}

auto ColorValidator::validate(QString& string, int&) const -> State
{
  if (const auto number = ToInteger(string))
  {
    if (number >= bottom() && number <= top())
    {
      return Acceptable;
    }
  }

  return Invalid;
}

}  // namespace tactile
