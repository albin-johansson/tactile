#include "property_name_validator.hpp"

#include "item_model_utils.hpp"
#include "tactile_error.hpp"

namespace tactile::gui {

property_name_validator::property_name_validator(QStandardItemModel* model,
                                                 QObject* parent)
    : QValidator{parent}
    , m_model{model}
{
  if (!m_model) {
    throw tactile_error{"Property validator requires non-null model!"};
  }
}

auto property_name_validator::validate(QString& input, int&) const
    -> QValidator::State
{
  if (input.isEmpty()) {
    return Invalid;
  }

  bool invalid{};

  viewmodel::visit_items(m_model, 0, [&](QStandardItem* item) {
    if (item->text() == input) {
      invalid = true;
    }
  });

  if (invalid) {
    return Invalid;
  } else {
    return Acceptable;
  }
}

}  // namespace tactile::gui
