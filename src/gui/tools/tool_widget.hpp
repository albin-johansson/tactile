#pragma once

#include <QButtonGroup>  // QButtonGroup
#include <QWidget>       // QWidget

#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, ToolWidget)

namespace tactile {

class ToolWidget final : public QWidget
{
  Q_OBJECT

 public:
  explicit ToolWidget(QWidget* parent = nullptr);

  ~ToolWidget() noexcept override;

  void EnableTools();

  void DisableTools();

 signals:
  void S_StampEnabled();
  void S_BucketEnabled();
  void S_EraserEnabled();

 public slots:
  void OnEnableStamp();
  void OnEnableEraser();
  void OnEnableBucket();

 private:
  Unique<Ui::ToolWidget> mUi;
  QButtonGroup* mGroup{};

  void SetToolsEnabled(bool enabled);
};

}  // namespace tactile
