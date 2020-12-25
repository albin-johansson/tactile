#pragma once

#include <QListView>

namespace tactile::gui {

class layer_list_view final : public QListView
{
  Q_OBJECT

 public:
  explicit layer_list_view(QWidget* parent = nullptr);

 protected:
  void selectionChanged(const QItemSelection& selected,
                        const QItemSelection& deselected) override;

  void mousePressEvent(QMouseEvent* event) override;
};

}
