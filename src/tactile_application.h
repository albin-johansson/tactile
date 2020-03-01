#pragma once
#include <QApplication>

namespace tactile {

class TactileApplication final : public QApplication {
 public:
  TactileApplication(int argc, char** argv);
};

}  // namespace tactile
