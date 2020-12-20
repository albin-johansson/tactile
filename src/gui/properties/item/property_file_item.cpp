#include "property_file_item.hpp"

#include <QLayout>

#include "file_value_widget.hpp"
#include "property_file_dialog.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

property_file_item::property_file_item(const QString& name,
                                       const core::property& property,
                                       QTreeWidgetItem* parent)
    : property_tree_item{name, parent}
    , m_valueWidget{new file_value_widget{}}
{
  setToolTip(0, TACTILE_QSTRING(u"(file)"));
  setText(1, TACTILE_QSTRING(u"N/A"));

  QObject::connect(m_valueWidget, &file_value_widget::spawn_dialog, [this] {
    spawn_dialog();
  });

  Q_ASSERT(treeWidget());
  treeWidget()->setItemWidget(this, 1, m_valueWidget);

  set_value(property);
}

void tactile::gui::property_file_item::spawn_dialog()
{
  property_file_dialog::spawn([this](const QString& path) {
    const QFileInfo file{path};
    set_value(core::property{file});
  });
}

void property_file_item::enable_focus_view()
{
  if (is_value_editable()) {
    m_valueWidget->set_visible(true);
  }
}

void property_file_item::enable_idle_view()
{
  m_valueWidget->set_visible(false);
}

void property_file_item::set_value(const core::property& property)
{
  if (property.has_value()) {
    Q_ASSERT(property.is<QFileInfo>());

    const auto& file = property.as<QFileInfo>();
    m_valueWidget->set_path(file);
    setText(1, file.fileName());
  } else {
    setText(1, TACTILE_QSTRING(u"N/A"));
  }
}

auto property_file_item::property_type() const noexcept -> core::property::type
{
  return core::property::file;
}

auto property_file_item::get_value_widget() -> QLineEdit*
{
  return nullptr;
}

}  // namespace tactile::gui
