#include "layer_widget.hpp"

#include <QMenu>
#include <QMouseEvent>
#include <QRadioButton>

#include "layer_item.hpp"
#include "layer_item_context_menu.hpp"
#include "ui_layer_widget.h"

namespace tactile::gui {

layer_widget::layer_widget(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::layer_widget{}}
{
  m_ui->setupUi(this);
  m_ui->layerList->setContextMenuPolicy(Qt::CustomContextMenu);

  // clang-format off
  connect(m_ui->layerList, &QListWidget::customContextMenuRequested, this, &layer_widget::trigger_layer_item_context_menu);
  connect(m_ui->newLayerButton, &QPushButton::pressed, this, &layer_widget::ui_requested_new_layer);
  connect(m_ui->removeLayerButton, &QPushButton::pressed, this, &layer_widget::ui_requested_remove_layer);
  connect(m_ui->layerList, &QListWidget::currentItemChanged, this, &layer_widget::current_item_changed);
  connect(m_ui->layerList, &QListWidget::itemChanged, this, &layer_widget::item_changed);
  // clang-format on

  connect(m_ui->upButton, &QPushButton::pressed, [this] {
    if (auto* item = current_item()) {
      emit ui_move_layer_up(item->layer());
    }
  });
  connect(m_ui->downButton, &QPushButton::pressed, [this] {
    if (auto* item = current_item()) {
      emit ui_move_layer_down(item->layer());
    }
  });
  connect(m_ui->visibleButton, &QPushButton::toggled, [this](bool visible) {
    if (auto* item = current_item()) {
      emit ui_set_layer_visibility(item->layer(), visible);
    }
  });
  connect(m_ui->opacitySpinBox,
          &QDoubleSpinBox::valueChanged,
          [this](double value) {
            if (auto* item = current_item()) {
              emit ui_set_layer_opacity(item->layer(), value);
            }
          });
}

void layer_widget::trigger_layer_item_context_menu(const QPoint& pos)
{
  if (m_ui->layerList->itemAt(pos)) {
    layer_item_context_menu menu{this};

    menu.set_visibility_enabled(m_ui->visibleButton->isEnabled());
    menu.set_move_up_enabled(m_ui->upButton->isEnabled());
    menu.set_move_down_enabled(m_ui->downButton->isEnabled());
    menu.set_remove_enabled(m_ui->removeLayerButton->isEnabled());

    connect(&menu, &layer_item_context_menu::toggle_visibility, [this] {
      m_ui->visibleButton->toggle();
    });

    connect(&menu, &layer_item_context_menu::move_layer_up, [this] {
      m_ui->upButton->click();
    });

    connect(&menu, &layer_item_context_menu::move_layer_down, [this] {
      m_ui->downButton->click();
    });

    connect(&menu, &layer_item_context_menu::remove_layer, [this] {
      emit ui_requested_remove_layer();
    });

    menu.exec(mapToGlobal(pos));
  }
}

layer_widget::~layer_widget() noexcept
{
  delete m_ui;
}

void layer_widget::added_layer(layer_id id, const core::layer& layer)
{
  add_layer(id, layer);
}

void layer_widget::removed_layer(layer_id id)
{
  if (auto* item = item_for_layer_id(id)) {
    m_ui->layerList->removeItemWidget(item);
    delete item;
  }
  update_possible_actions();
}

void layer_widget::selected_layer(layer_id id, const core::layer& layer)
{
  Q_ASSERT(item_for_layer_id(id) != nullptr);

  m_ui->visibleButton->setChecked(layer.visible());
  m_ui->opacitySpinBox->setValue(layer.opacity());
}

void layer_widget::selected_map(const core::map_document& document)
{
  m_ui->layerList->clear();
  m_nameSuffix = 1;

  document.each_layer([this](layer_id id, const core::layer& layer) {
    add_layer(id, layer);
  });

  if (const auto id = document.current_layer_id(); id) {
    if (auto* item = item_for_layer_id(*id)) {
      selected_layer(*id, document.get_layer(*id));
      m_ui->layerList->setCurrentItem(item);
    }
  }
}

void layer_widget::moved_layer_back(layer_id id)
{
  if (const auto* item = item_for_layer_id(id)) {
    const auto row = m_ui->layerList->row(item);
    const auto newRow = row + 1;

    m_ui->layerList->insertItem(newRow, m_ui->layerList->takeItem(row));
    m_ui->layerList->setCurrentRow(newRow);
    update_possible_actions();

    Q_ASSERT(m_ui->layerList->row(item) == newRow);
  }
}

void layer_widget::moved_layer_forward(layer_id id)
{
  if (const auto* item = item_for_layer_id(id)) {
    const auto row = m_ui->layerList->row(item);
    const auto newRow = row - 1;

    m_ui->layerList->insertItem(newRow, m_ui->layerList->takeItem(row));
    m_ui->layerList->setCurrentRow(newRow);
    update_possible_actions();

    Q_ASSERT(m_ui->layerList->row(item) == newRow);
  }
}

void layer_widget::add_layer(layer_id id, const core::layer& layer)
{
  if (!layer.name().isEmpty()) {
    m_ui->layerList->addItem(new layer_item{layer.name(), id, m_ui->layerList});
  } else {
    m_ui->layerList->addItem(new layer_item{
        QStringLiteral(u"Layer ") + QString::number(m_nameSuffix),
        id,
        m_ui->layerList});
    ++m_nameSuffix;
  }
  update_possible_actions();
}

auto layer_widget::item_for_layer_id(layer_id id) -> layer_item*
{
  const auto count = m_ui->layerList->count();
  for (auto row = 0; row < count; ++row) {
    if (auto* item = dynamic_cast<layer_item*>(m_ui->layerList->item(row))) {
      if (item->layer() == id) {
        return item;
      }
    }
  }
  return nullptr;
}

auto layer_widget::current_item() const -> const layer_item*
{
  return dynamic_cast<const layer_item*>(m_ui->layerList->currentItem());
}

void layer_widget::update_possible_actions()
{
  const auto itemCount = m_ui->layerList->count();
  const auto currentRow = m_ui->layerList->currentRow();

  m_ui->removeLayerButton->setEnabled(itemCount > 1);
  m_ui->upButton->setEnabled(itemCount > 1 && currentRow != 0);
  m_ui->downButton->setEnabled(itemCount > 1 && (currentRow != itemCount - 1));
}

void layer_widget::current_item_changed(QListWidgetItem* current,
                                        QListWidgetItem* previous)
{
  if (current != previous) {
    if (auto* item = dynamic_cast<layer_item*>(current)) {
      emit ui_selected_layer(item->layer());
    }
  }
  update_possible_actions();
}

void layer_widget::item_changed(QListWidgetItem* item)
{
  if (auto* layerItem = dynamic_cast<layer_item*>(item)) {
    emit ui_set_layer_name(layerItem->layer(), item->text());
  }
}

}  // namespace tactile::gui
