#pragma once

#include <QButtonGroup>  // QButtonGroup
#include <QWidget>       // QWidget

#include "smart_pointers.hpp"
#include "tactile_declare_ui.hpp"

TACTILE_DECLARE_UI(ToolWidget)

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
