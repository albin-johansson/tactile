#include "property_file_item.hpp"

#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QToolButton>

#include "property_file_dialog.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {
namespace {

[[nodiscard]] auto make_edit() -> QLineEdit*
{
  auto* edit = new QLineEdit{};

  edit->setFrame(false);
  edit->setReadOnly(true);
  edit->setAutoFillBackground(true);
  edit->setText(TACTILE_QSTRING(u"N/A"));
  edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  return edit;
}

template <std::invocable T>
[[nodiscard]] auto make_button(T&& onPressed) -> QToolButton*
{
  auto* button = new QToolButton{};

  button->setText(TACTILE_QSTRING(u"..."));
  button->setToolTip(QObject::tr("Select file"));

  QObject::connect(button, &QToolButton::pressed, onPressed);

  return button;
}

}  // namespace

property_file_item::property_file_item(const QString& name,
                                       const core::property& property,
                                       QTreeWidgetItem* parent)
    : property_tree_item{parent}
{
  Q_ASSERT(parent);
  setText(0, name);
  setToolTip(0, TACTILE_QSTRING(u"(file)"));

  auto* wrapper = new QWidget{};
  wrapper->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  wrapper->setAutoFillBackground(true);

  auto* layout = new QHBoxLayout{};
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(2);

  m_edit = make_edit();

  layout->addWidget(m_edit);
  layout->addWidget(make_button([this] {
    spawn_dialog();
  }));
  wrapper->setLayout(layout);

  Q_ASSERT(treeWidget());
  treeWidget()->setItemWidget(this, 1, wrapper);

  set_value(property);
}

void tactile::gui::property_file_item::spawn_dialog()
{
  property_file_dialog::spawn([this](const QString& path) {
    m_edit->setText(path);
  });
}

void property_file_item::set_value(const core::property& property)
{
  Q_ASSERT(property.is<QFileInfo>());
  if (auto* widget = get_value_widget()) {
    widget->setText(property.as<QFileInfo>().path());
  }
}

auto property_file_item::property_type() const noexcept -> core::property::type
{
  return core::property::file;
}

auto property_file_item::is_inline() const noexcept -> bool
{
  return false;
}

auto property_file_item::get_value_widget() -> QLineEdit*
{
  return m_edit;
}

}  // namespace tactile::gui
