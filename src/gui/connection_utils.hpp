#pragma once

#include <QAction>
#include <QDockWidget>
#include <QObject>

namespace tactile {

template <typename Sender, typename Signal, typename Receiver, typename Slot>
void connect(Sender&& sender, Signal&& signal, Receiver&& receiver, Slot&& slot)
{
  QObject::connect(sender, signal, receiver, slot);
}

template <typename Sender, typename Receiver, typename Slot>
void on_triggered(Sender&& sender, Receiver&& receiver, Slot&& slot)
{
  connect(sender, &QAction::triggered, receiver, slot);
}

}  // namespace tactile
