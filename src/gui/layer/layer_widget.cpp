#include "layer_widget.hpp"

#include <QMenu>
#include <QMouseEvent>
#include <QRadioButton>

#include "layer_item.hpp"
#include "ui_layer_widget.h"

namespace tactile::gui {

layer_widget::layer_widget(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::layer_widget{}}
{
  m_ui->setupUi(this);

  connect(m_ui->newLayerButton,
          &QPushButton::pressed,
          this,
          &layer_widget::ui_requested_new_layer);

  connect(m_ui->removeLayerButton,
          &QPushButton::pressed,
          this,
          &layer_widget::ui_requested_remove_layer);

  connect(m_ui->layerList,
          &QListWidget::currentItemChanged,
          [this](QListWidgetItem* current, QListWidgetItem* previous) {
            if (current != previous) {
              if (auto* item = dynamic_cast<layer_item*>(current)) {
                emit ui_selected_layer(item->layer());
              }
            }
            update_possible_actions();
          });

  m_ui->layerList->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_ui->layerList,
          &QListWidget::customContextMenuRequested,
          this,
          &layer_widget::trigger_layer_item_context_menu);
}

void layer_widget::trigger_layer_item_context_menu(const QPoint& pos)
{
  if (auto* item = m_ui->layerList->itemAt(pos)) {
    QMenu menu{this};

    QAction remove{tr("Remove layer"), this};
    remove.setEnabled(m_ui->layerList->count() > 1);
    connect(&remove, &QAction::triggered, [this](bool checked) {
      emit ui_requested_remove_layer();
    });

    QAction toggleVisibility{tr("Toggle visibility"), this};

    menu.addAction(&toggleVisibility);
    menu.addAction(menu.addSeparator());
    menu.addAction(&remove);

    menu.exec(mapToGlobal(pos));
  }
}

layer_widget::~layer_widget() noexcept
{
  delete m_ui;
}

void layer_widget::added_layer(layer_id id, const core::layer& layer)
{
  add_layer(id);
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
  //  m_ui->opacitySpinBox->setValue(layer.opacity()); // TODO
}

void layer_widget::selected_map(const core::map_document& document)
{
  m_ui->layerList->clear();
  m_nameSuffix = 1;

  document.each_layer(
      [this](layer_id id, const core::layer&) { add_layer(id); });

  if (const auto id = document.current_layer_id(); id) {
    if (auto* item = item_for_layer_id(*id)) {
      selected_layer(*id, document.get_layer(*id));
      m_ui->layerList->setCurrentItem(item);
    }
  }
}

void layer_widget::add_layer(layer_id id)
{
  m_ui->layerList->addItem(
      new layer_item{QStringLiteral(u"Layer ") + QString::number(m_nameSuffix),
                     id,
                     m_ui->layerList});
  ++m_nameSuffix;
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

void layer_widget::update_possible_actions()
{
  m_ui->removeLayerButton->setEnabled(m_ui->layerList->count() > 1);
}

}  // namespace tactile::gui
