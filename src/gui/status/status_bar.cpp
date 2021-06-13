#include "status_bar.hpp"

#include "map_document.hpp"

namespace tactile {

StatusBar::StatusBar(QWidget* parent)
    : QStatusBar{parent}
    , mLayerBox{new QComboBox{this}}
    , mMouseXLabel{new QLabel{QStringLiteral(u"X:")}}
    , mMouseYLabel{new QLabel{QStringLiteral(u"Y:")}}
    , mMouseXValueLabel{new QLabel{}}
    , mMouseYValueLabel{new QLabel{}}
{
  setObjectName(QStringLiteral(u"StatusBar"));

  mLayerBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

  addWidget(mMouseXLabel);
  addWidget(mMouseYLabel);
  addWidget(mMouseXValueLabel);
  addWidget(mMouseYValueLabel);
  addPermanentWidget(mLayerBox);

  // clang-format off
  connect(mLayerBox, &QComboBox::currentIndexChanged, this, &StatusBar::OnCurrentIndexChanged);
  // clang-format on
}

void StatusBar::SetLayerComboBoxVisible(const bool visible)
{
  if (visible)
  {
    addPermanentWidget(mLayerBox);
    mLayerBox->show();
  }
  else
  {
    removeWidget(mLayerBox);
  }
}

void StatusBar::SetMouseInfoVisible(const bool visible)
{
  if (visible)
  {
    addWidget(mMouseXLabel);
    addWidget(mMouseXValueLabel);
    addWidget(mMouseYLabel);
    addWidget(mMouseYValueLabel);

    mMouseXLabel->show();
    mMouseXValueLabel->show();
    mMouseYLabel->show();
    mMouseYValueLabel->show();
  }
  else
  {
    removeWidget(mMouseXLabel);
    removeWidget(mMouseXValueLabel);
    removeWidget(mMouseYLabel);
    removeWidget(mMouseYValueLabel);
  }
}

void StatusBar::SetCurrentLayer(layer_id id)
{
  QSignalBlocker blocker{mLayerBox};  // avoids switch layer requests

  const auto index = mLayerBox->findData(id.get());
  mLayerBox->setCurrentIndex(index);
}

void StatusBar::OnSwitchedMap(const core::MapDocument& document)
{
  QSignalBlocker blocker{mLayerBox};  // avoids switch layer requests

  mLayerBox->clear();
  document.EachLayer(
      [this](const layer_id id, const Shared<core::ILayer>& layer) {
        mLayerBox->addItem(layer->Name(), id.get());
      });
  SetLayerComboBoxVisible(true);

  const auto currentId = document.CurrentLayerId().value();
  const auto index = mLayerBox->findData(currentId.get());
  mLayerBox->setCurrentIndex(index);
}

void StatusBar::OnAddedLayer(layer_id id, const QString& name)
{
  QSignalBlocker blocker{mLayerBox};  // avoids switch layer requests
  mLayerBox->addItem(name, id.get());
}

void StatusBar::SetLayerName(layer_id id, const QString& name)
{
  QSignalBlocker blocker{mLayerBox};  // avoids switch layer requests

  const auto index = mLayerBox->findData(id.get());
  Q_ASSERT(index != -1);

  mLayerBox->setItemText(index, name);
}

void StatusBar::OnRemovedLayer(layer_id id)
{
  QSignalBlocker blocker{mLayerBox};  // avoids switch layer requests

  const auto index = mLayerBox->findData(id.get());
  Q_ASSERT(index != -1);

  mLayerBox->removeItem(index);
}

void StatusBar::OnMouseMoved(const QPointF& pos)
{
  mMouseXValueLabel->setNum(pos.x());
  mMouseYValueLabel->setNum(pos.y());
}

void StatusBar::OnCurrentIndexChanged(const int index)
{
  const layer_id id{mLayerBox->itemData(index).value<int>()};
  emit S_SelectLayerRequest(id);
}

}  // namespace tactile
