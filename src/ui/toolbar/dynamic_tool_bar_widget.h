#pragma once

#include <QWidget>

class QLayout;

namespace tactile {

class StampToolbar;

class DynamicToolBarWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit DynamicToolBarWidget(QWidget* parent = nullptr);

  ~DynamicToolBarWidget() noexcept override;

 signals:
  void req_stamp_tool();

  void req_bucket_tool();

  void req_eraser_tool();

 private:
  QLayout* m_layout;
  StampToolbar* m_stampToolbar;
};

}  // namespace tactile
