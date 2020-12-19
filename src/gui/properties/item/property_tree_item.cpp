#include "property_tree_item.hpp"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <utility>  // move

#include "color_preview_button.hpp"

namespace tactile::gui {
namespace {

[[nodiscard]] auto inline_widget_for_type(const core::property::type type,
                                          const int index) -> QWidget*
{
  const auto role = (index % 2 == 0) ? QPalette::Base : QPalette::AlternateBase;
  switch (type) {
    case core::property::string:
    case core::property::file:  // TODO line edit with "browse" button?
      [[fallthrough]];
    case core::property::object:  // TODO RO line edit with browsing options
      return nullptr;

    case core::property::color:
      return new color_preview_button{Qt::black};

    case core::property::integer: {
      auto* edit = new QLineEdit{};
      edit->setFrame(false);
      edit->setValidator(new QIntValidator{});
      edit->setBackgroundRole(role);
      return edit;
    }
    case core::property::floating: {
      auto* edit = new QLineEdit{};
      edit->setFrame(false);
      edit->setValidator(new QDoubleValidator{});
      edit->setBackgroundRole(role);
      return edit;
    }
    case core::property::boolean: {
      auto* box = new QCheckBox{};
      box->setBackgroundRole(role);
      return box;
    }
    default:
      throw tactile_error{"Did not recognize property type for item widget!"};
  }
}

}  // namespace

void property_tree_item::set_name_editable(const bool editable)
{
  Q_ASSERT(treeWidget());
  m_isValueEditable = editable;

  auto newFlags = flags();
  newFlags.setFlag(Qt::ItemIsEditable, editable);
  setFlags(newFlags);
}

void property_tree_item::set_value_editable(const bool editable)
{
  m_isValueEditable = editable;
  if (auto* widget = get_value_widget()) {
    widget->setEnabled(m_isValueEditable);
  }
}

auto property_tree_item::is_name_editable() const noexcept -> bool
{
  return m_isNameEditable;
}

auto property_tree_item::is_value_editable() const noexcept -> bool
{
  return m_isValueEditable;
}

}  // namespace tactile::gui
