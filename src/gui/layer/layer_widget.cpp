#include "layer_widget.hpp"

#include "icons.hpp"
#include "init_ui.hpp"
#include "layer_item_context_menu.hpp"
#include "layer_list_view.hpp"
#include "layer_widget_context_menu.hpp"
#include "ui_layer_widget.h"

namespace tactile::gui {

// FIXME status bar layer combobox is not updated directly when layer name is
// changed

layer_widget::layer_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{init_ui<Ui::layer_widget>(this)}
    , m_listView{new layer_list_view{this}}
    , m_addLayerMenu{new add_layer_context_menu{this}}
    , m_widgetMenu{new layer_widget_context_menu{m_addLayerMenu, this}}
    , m_itemMenu{new layer_item_context_menu{this}}
{
  m_ui->gridLayout->addWidget(m_listView, 0, 1);
  m_ui->gridLayout->removeWidget(m_ui->opacitySlider);
  m_ui->gridLayout->addWidget(m_ui->opacitySlider, 0, 2);

  m_ui->removeLayerButton->setEnabled(false);
  m_ui->duplicateButton->setEnabled(false);
  m_ui->upButton->setEnabled(false);
  m_ui->downButton->setEnabled(false);
  m_ui->visibleButton->setEnabled(false);
  m_ui->opacitySlider->setEnabled(false);

  // clang-format off
  connect(m_listView, &QWidget::customContextMenuRequested,
          this, &layer_widget::spawn_context_menu);

  connect(m_listView, &layer_list_view::selection_changed,
          this, &layer_widget::when_selection_changed);

  connect(m_addLayerMenu, &add_layer_context_menu::add_tile_layer,
          this, &layer_widget::new_tile_layer_requested);

  connect(m_addLayerMenu, &add_layer_context_menu::add_object_layer,
          this, &layer_widget::new_object_layer_requested);
  // clang-format on
}

layer_widget::~layer_widget() noexcept
{
  delete m_ui;
}

void layer_widget::selected_map(const core::map_document& document,
                                const shared<vm::layer_model>& model)
{
  if (m_model) {
    m_model->disconnect(m_listView);
  }

  Q_ASSERT(!model->parent());
  m_listView->setModel(model.get());
  Q_ASSERT(!model->parent());

  m_model = model;
}

void layer_widget::spawn_context_menu(const QPoint& pos)
{
  if (m_listView->selectionModel()->selection().empty()) {
    m_widgetMenu->exec(mapToGlobal(pos));
  } else {
    m_itemMenu->exec(mapToGlobal(pos));
  }
}

[[maybe_unused]] //
void layer_widget::when_selection_changed(const maybe<QModelIndex>& selected,
                                          const maybe<QModelIndex>& deselected)
{
  if (!m_model) {
    return;
  }

  m_ui->opacitySlider->setEnabled(selected.has_value());
  m_ui->duplicateButton->setEnabled(selected.has_value());
  m_ui->visibleButton->setEnabled(selected.has_value());

  const auto nLayers = m_model->rowCount();
  const maybe<int> index = selected ? selected->row() : maybe<int>{};
  m_ui->upButton->setEnabled(index && index != 0);
  m_ui->downButton->setEnabled(index && index != nLayers - 1);
  m_ui->removeLayerButton->setEnabled(nLayers != 1);

  if (selected) {
    QSignalBlocker opacityBlocker{m_ui->opacitySlider};
    m_ui->opacitySlider->setValue(
        static_cast<int>(m_model->opacity(*selected) * 100.0));

    m_ui->visibleButton->setIcon(
        m_model->visible(*selected) ? icons::visible() : icons::invisible());

    m_model->select(*selected);
  }
}

void layer_widget::new_tile_layer_requested()
{
  Q_ASSERT(m_model);
  m_model->add_tile_layer();
}

void layer_widget::new_object_layer_requested()
{
  Q_ASSERT(m_model);
  m_model->add_object_layer();
}

[[maybe_unused]] //
void layer_widget::on_newLayerButton_pressed()
{
  auto pos = m_ui->newLayerButton->pos();

  const auto size = m_ui->newLayerButton->size() / 2.0;
  pos += QPoint{size.width(), size.height()};

  m_addLayerMenu->exec(mapToGlobal(pos));
}

[[maybe_unused]] //
void layer_widget::on_removeLayerButton_pressed()
{
  m_model->remove(m_listView->currentIndex());
}

[[maybe_unused]] //
void layer_widget::on_upButton_pressed()
{
  m_model->move_up(m_listView->currentIndex());
}

[[maybe_unused]] //
void layer_widget::on_downButton_pressed()
{
  m_model->move_down(m_listView->currentIndex());
}

[[maybe_unused]] //
void layer_widget::on_duplicateButton_pressed()
{
  m_model->duplicate(m_listView->currentIndex());
}

[[maybe_unused]] //
void layer_widget::on_visibleButton_toggled(const bool visible)
{
  m_model->set_visible(m_listView->currentIndex(), visible);
  m_ui->visibleButton->setIcon(visible ? icons::visible() : icons::invisible());
}

[[maybe_unused]] //
void layer_widget::on_opacitySlider_valueChanged(const int value)
{
  m_model->set_opacity(m_listView->currentIndex(),
                       static_cast<double>(value) / 100.0);
}

}  // namespace tactile::gui
