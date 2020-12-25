#include "layer_widget.hpp"

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
    , m_itemContextMenu{new layer_item_context_menu{this}}
    , m_widgetContextMenu{new layer_widget_context_menu{this}}
{
  m_ui->gridLayout->addWidget(m_listView, 0, 1);

  m_ui->removeLayerButton->setEnabled(false);
  m_ui->duplicateButton->setEnabled(false);
  m_ui->upButton->setEnabled(false);
  m_ui->downButton->setEnabled(false);
  m_ui->visibleButton->setEnabled(false);

  //  m_ui->layerList->setContextMenuPolicy(Qt::CustomContextMenu);

  // clang-format off
//  connect(m_itemContextMenu, &layer_item_context_menu::toggle_visibility,
//          m_ui->visibleButton, &QAbstractButton::toggle);
//
//  connect(m_itemContextMenu, &layer_item_context_menu::move_layer_up,
//          m_ui->upButton, &QAbstractButton::click);
//
//  connect(m_itemContextMenu, &layer_item_context_menu::move_layer_down,
//          m_ui->downButton, &QAbstractButton::click);
//
//  connect(m_itemContextMenu, &layer_item_context_menu::duplicate_layer,
//          m_ui->duplicateButton, &QAbstractButton::click);
//
//  connect(m_itemContextMenu, &layer_item_context_menu::remove_layer,
//          m_ui->removeLayerButton, &QAbstractButton::click);
//
  connect(m_widgetContextMenu, &layer_widget_context_menu::add_tile_layer,
          this, &layer_widget::new_tile_layer_requested);

  connect(m_widgetContextMenu, &layer_widget_context_menu::add_object_layer,
          this, &layer_widget::new_object_layer_requested);
  // clang-format on
}

layer_widget::~layer_widget() noexcept
{
  delete m_ui;
}

void layer_widget::selected_map(const map_id id,
                                const core::map_document& document,
                                const shared<vm::layer_model>& model)
{
  qDebug("layer_widget > Selected map %i", id.get());

  if (m_model) {
    m_model->disconnect(m_listView);
  }

  m_currentMap = id;

  Q_ASSERT(!model->parent());
  m_listView->setModel(model.get());
  Q_ASSERT(!model->parent());

  m_model = model;

  //  m_ui->layerList->clear();
  //
  //  if (!m_suffixes.contains(mapId)) {
  //    m_suffixes.emplace(mapId, 1);
  //  }
  //
  //  document.each_layer(
  //      [this](const layer_id layerId, const core::shared_layer& layer) {
  //        add_tile_layer(layerId, *layer);
  //      });
  //
  //  if (const auto id = document.current_layer_id()) {
  //    if (auto* item = item_for_layer_id(*id)) {
  //      selected_layer(*id, document.get_layer(*id));
  //      m_ui->layerList->setCurrentItem(item);
  //    }
  //  }
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

  m_widgetContextMenu->exec(mapToGlobal(pos));
}

[[maybe_unused]] //
void layer_widget::on_removeLayerButton_pressed()
{}

[[maybe_unused]] //
void layer_widget::on_upButton_pressed()
{}

[[maybe_unused]] //
void layer_widget::on_downButton_pressed()
{}

[[maybe_unused]] //
void layer_widget::on_duplicateButton_pressed()
{}

[[maybe_unused]] //
void layer_widget::on_visibleButton_toggled(const bool visible)
{}

[[maybe_unused]] //
void layer_widget::on_opacitySlider_valueChanged(const int value)
{}

}  // namespace tactile::gui
