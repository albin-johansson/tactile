#include "layer_widget.hpp"

#include <QMenu>
#include <QMouseEvent>
#include <QRadioButton>

#include "icons.hpp"
#include "layer_item.hpp"
#include "layer_item_context_menu.hpp"
#include "tactile_qstring.hpp"
#include "ui_layer_widget.h"

namespace tactile::gui {

layer_widget::layer_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{new Ui::layer_widget{}}
    , m_contextMenu{new layer_item_context_menu{this}}
{
  m_ui->setupUi(this);
  m_ui->layerList->setContextMenuPolicy(Qt::CustomContextMenu);

  // clang-format off
  connect(m_contextMenu, &layer_item_context_menu::toggle_visibility, m_ui->visibleButton, &QAbstractButton::toggle);
  connect(m_contextMenu, &layer_item_context_menu::move_layer_up, m_ui->upButton, &QAbstractButton::click);
  connect(m_contextMenu, &layer_item_context_menu::move_layer_down, m_ui->downButton, &QAbstractButton::click);
  connect(m_contextMenu, &layer_item_context_menu::duplicate_layer, m_ui->duplicateButton, &QAbstractButton::click);
  connect(m_contextMenu, &layer_item_context_menu::remove_layer, m_ui->removeLayerButton, &QAbstractButton::click);

  connect(m_ui->layerList, &QListWidget::customContextMenuRequested, this, &layer_widget::trigger_layer_item_context_menu);
  connect(m_ui->newLayerButton, &QPushButton::pressed, this, &layer_widget::ui_add_layer);
  connect(m_ui->layerList, &QListWidget::currentItemChanged, this, &layer_widget::current_item_changed);
  connect(m_ui->layerList, &QListWidget::itemChanged, this, &layer_widget::item_changed);
  // clang-format on

  connect(m_ui->removeLayerButton, &QPushButton::pressed, [this] {
    if (auto* item = current_item()) {
      emit ui_remove_layer(item->layer());
    }
  });

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

  connect(m_ui->duplicateButton, &QPushButton::pressed, [this] {
    if (auto* item = current_item()) {
      m_duplicateTargetRow = m_ui->layerList->row(item) + 1;
      emit ui_duplicate_layer(item->layer());
    }
  });

  connect(m_ui->visibleButton,
          &QPushButton::toggled,
          [this](const bool visible) {
            if (auto* item = current_item()) {
              emit ui_set_layer_visibility(item->layer(), visible);
              m_ui->visibleButton->setIcon(visible ? icons::visible()
                                                   : icons::invisible());
            }
          });

  connect(m_ui->opacitySlider, &QSlider::valueChanged, [this](const int value) {
    if (auto* item = current_item()) {
      const auto dValue = static_cast<double>(value) / 100.0;
      emit ui_set_layer_opacity(item->layer(), dValue);
    }
  });
}

layer_widget::~layer_widget() noexcept
{
  delete m_ui;
}

void layer_widget::trigger_layer_item_context_menu(const QPoint& pos)
{
  if (m_ui->layerList->itemAt(pos)) {
    m_contextMenu->set_visibility_enabled(m_ui->visibleButton->isEnabled());
    m_contextMenu->set_move_up_enabled(m_ui->upButton->isEnabled());
    m_contextMenu->set_move_down_enabled(m_ui->downButton->isEnabled());
    m_contextMenu->set_remove_enabled(m_ui->removeLayerButton->isEnabled());
    m_contextMenu->exec(mapToGlobal(pos));
  }
}

void layer_widget::added_layer(const layer_id id, const core::tile_layer& layer)
{
  add_layer(id, layer);
}

void layer_widget::added_duplicated_layer(const layer_id id,
                                          const core::tile_layer& layer)
{
  add_layer(id, layer);
  auto* item = item_for_layer_id(id);
  Q_ASSERT(item);

  const auto name = layer.name() + tr(" (Copy)");
  item->setText(name);
  emit ui_set_layer_name(id, name);

  // The layer item is initially added to the end, move up until it's in place
  const auto target = m_duplicateTargetRow.value();
  while (m_ui->layerList->row(item) != target) {
    emit ui_move_layer_up(id);
  }

  m_duplicateTargetRow.reset();
}

void layer_widget::removed_layer(layer_id id)
{
  if (auto* item = item_for_layer_id(id)) {
    m_ui->layerList->removeItemWidget(item);
    delete item;
  }
  update_possible_actions();
}

void layer_widget::selected_layer(const layer_id id,
                                  const core::tile_layer& layer)
{
  Q_ASSERT(item_for_layer_id(id) != nullptr);

  m_ui->visibleButton->setChecked(layer.visible());
  m_ui->visibleButton->setIcon(layer.visible() ? icons::visible()
                                               : icons::invisible());
  m_ui->opacitySlider->setValue(static_cast<int>(layer.opacity() * 100.0));
}

void layer_widget::selected_map(const map_id mapId,
                                const core::map_document& document)
{
  m_currentMap = mapId;
  m_ui->layerList->clear();

  if (!m_suffixes.contains(mapId)) {
    m_suffixes.emplace(mapId, 1);
  }

  document.each_layer(
      [this](const layer_id layerId, const core::tile_layer& layer) {
        add_layer(layerId, layer);
      });

  if (const auto id = document.current_layer_id()) {
    if (auto* item = item_for_layer_id(*id)) {
      selected_layer(*id, document.get_layer(*id));
      m_ui->layerList->setCurrentItem(item);
    }
  }
}

void layer_widget::moved_layer_back(const layer_id id)
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

void layer_widget::moved_layer_forward(const layer_id id)
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

void layer_widget::add_layer(const layer_id id, const core::tile_layer& layer)
{
  QString name;
  if (layer.name().isEmpty()) {
    auto& suffix = m_suffixes.at(m_currentMap.value());
    name = TACTILE_QSTRING(u"Layer ") + QString::number(suffix);
    ++suffix;
  } else {
    name = layer.name();
  }

  auto* item = new layer_item{name, id, m_ui->layerList};

  m_ui->layerList->addItem(item);
  update_possible_actions();
}

auto layer_widget::item_for_layer_id(const layer_id id) -> layer_item*
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
      emit ui_select_layer(item->layer());
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
