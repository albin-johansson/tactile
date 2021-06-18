#include "property_name_validator.hpp"

#include <QStandardItemModel>  // QStandardItemModel

#include "item_model_utils.hpp"
#include "tactile_error.hpp"

namespace tactile {

PropertyNameValidator::PropertyNameValidator(QStandardItemModel* model,
                                             QObject* parent)
    : QValidator{parent}
    , mModel{model}
{
  if (!mModel)
  {
    throw TactileError{"Property validator requires non-null model!"};
  }

  setObjectName(QStringLiteral(u"PropertyNameValidator"));
}

auto PropertyNameValidator::validate(QString& input, int&) const
    -> QValidator::State
{
  if (input.isEmpty())
  {
    return Invalid;
  }

  bool invalid{};

  vm::VisitItems(mModel, 0, [&](QStandardItem* item) {
    if (item->text() == input)
    {
      invalid = true;
    }
  });

  if (invalid)
  {
    return Invalid;
  }
  else
  {
    return Acceptable;
  }
}

}  // namespace tactile
