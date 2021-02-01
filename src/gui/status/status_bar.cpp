#include "status_bar.hpp"

#include <QComboBox>
#include <QLabel>

#include "tactile_qstring.hpp"

namespace tactile::gui {

status_bar::status_bar(QWidget* parent)
    : QStatusBar{parent}
    , m_layerBox{new QComboBox{this}}
    , m_mxLabel{new QLabel{TACTILE_QSTRING(u"X:")}}
    , m_myLabel{new QLabel{TACTILE_QSTRING(u"Y:")}}
    , m_mxValueLabel{new QLabel{}}
    , m_myValueLabel{new QLabel{}}
{
  m_layerBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

  addWidget(m_mxLabel);
  addWidget(m_myLabel);
  addWidget(m_mxValueLabel);
  addWidget(m_myValueLabel);
  addPermanentWidget(m_layerBox);

  connect(m_layerBox, &QComboBox::currentIndexChanged, [this](const int index) {
    const layer_id id{m_layerBox->itemData(index).value<int>()};
    emit select_layer_request(id);
  });
}

void status_bar::set_layer_combo_box_visible(const bool visible)
{
  if (visible) {
    addPermanentWidget(m_layerBox);
    m_layerBox->show();
  } else {
    removeWidget(m_layerBox);
  }
}

void status_bar::set_mouse_info_visible(const bool visible)
{
  if (visible) {
    addWidget(m_mxLabel);
    addWidget(m_mxValueLabel);
    addWidget(m_myLabel);
    addWidget(m_myValueLabel);

    m_mxLabel->show();
    m_mxValueLabel->show();
    m_myLabel->show();
    m_myValueLabel->show();
  } else {
    removeWidget(m_mxLabel);
    removeWidget(m_mxValueLabel);
    removeWidget(m_myLabel);
    removeWidget(m_myValueLabel);
  }
}

void status_bar::set_current_layer(const layer_id id)
{
  QSignalBlocker blocker{m_layerBox};  // avoids switch layer requests

  const auto index = m_layerBox->findData(id.get());
  m_layerBox->setCurrentIndex(index);
}

void status_bar::switched_map(const core::map_document& document)
{
  QSignalBlocker blocker{m_layerBox};  // avoids switch layer requests

  m_layerBox->clear();
  document.each_layer(
      [this](const layer_id id, const shared<core::layer>& layer) {
        m_layerBox->addItem(layer->name(), id.get());
      });
  set_layer_combo_box_visible(true);

  const auto currentId = document.current_layer_id().value();
  const auto index = m_layerBox->findData(currentId.get());
  m_layerBox->setCurrentIndex(index);
}

void status_bar::added_layer(const layer_id id, const QString& name)
{
  QSignalBlocker blocker{m_layerBox};  // avoids switch layer requests
  m_layerBox->addItem(name, id.get());
}

void status_bar::set_layer_name(const layer_id id, const QString& name)
{
  QSignalBlocker blocker{m_layerBox};  // avoids switch layer requests

  const auto index = m_layerBox->findData(id.get());
  Q_ASSERT(index != -1);

  m_layerBox->setItemText(index, name);
}

void status_bar::removed_layer(const layer_id id)
{
  QSignalBlocker blocker{m_layerBox};  // avoids switch layer requests

  const auto index = m_layerBox->findData(id.get());
  Q_ASSERT(index != -1);

  m_layerBox->removeItem(index);
}

void status_bar::mouse_moved(const QPointF& pos)
{
  m_mxValueLabel->setNum(pos.x());
  m_myValueLabel->setNum(pos.y());
}

}  // namespace tactile::gui
